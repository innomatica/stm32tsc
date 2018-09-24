#!/usr/bin/env python3
################################################################################
#
#   \file
#   \author     <a href="http://www.innomatic.ca">innomatic</a>
#   \brief      wxPython Terminal Window
#   \copyright  <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons Licence" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.
#

import statistics as st
import wx
from wxTerm import *
from wplGraph import *

#--------1---------2---------3---------4---------5---------6---------7---------8
##
# \brief Graph panel
class GraphPanel(wx.Panel):

    def __init__(self, *args, **kwgs):
        wx.Panel.__init__(self, *args, **kwgs)

        # plot canvas
        self.grpTouch = WplGraph(self)
        # control panel
        self.pnlControl = wx.Panel(self)

        # statistics display
        self.lscStats = wx.ListCtrl(self.pnlControl, style = wx.LC_REPORT,
                size = (200,-1))
        self.lscStats.InsertColumn(0,'Item',width=120)
        self.lscStats.InsertColumn(1, 'Value')
        self.sttDSize = wx.StaticText(self.pnlControl, label='Data Size')
        # data size dropdown
        self.choDSize = wx.Choice(self.pnlControl,
                choices=['100','200','500','1000'])

        # run button
        self.tglRun = wx.ToggleButton(self.pnlControl, label='RUN')

        sizer_h = wx.BoxSizer(wx.HORIZONTAL)
        sizer_h.Add(self.sttDSize, 0, wx.RIGHT|wx.EXPAND, 4)
        sizer_h.Add(self.choDSize, 1, wx.LEFT|wx.EXPAND, 4)

        sizer_v = wx.BoxSizer(wx.VERTICAL)
        sizer_v.Add(self.lscStats, 1, wx.ALL|wx.EXPAND, 4)
        sizer_v.Add(sizer_h, 0, wx.ALL|wx.EXPAND, 4)
        sizer_v.Add(self.tglRun, 0, wx.ALL|wx.EXPAND, 4)
        self.pnlControl.SetSizer(sizer_v)

        # event binding
        self.Bind(EVT_UPDATE_COMDATA, self.OnUpdateComData)
        self.choDSize.Bind(wx.EVT_CHOICE, self.OnNewDataSize)
        self.tglRun.Bind(wx.EVT_TOGGLEBUTTON, self.OnGraphRun)

        sizer_h = wx.BoxSizer(wx.HORIZONTAL)
        sizer_h.Add(self.grpTouch, 1, wx.ALL|wx.EXPAND, 4)
        sizer_h.Add(self.pnlControl, 0, wx.ALL|wx.EXPAND, 4)
        self.SetSizer(sizer_h)

        # raw data storage
        self.index = None
        self.SetGraphRange(100)
        self.choDSize.SetSelection(0)
        # packet decoder
        self.pd = PacketDecoder()
        self.pd.SetMode('payload')
        # graph colors
        self.grpColor = ['ORANGE RED','CORNFLOWER BLUE','DARK OLIVE GREEN',
                'VIOLET RED','SLATE BLUE','SPRING GREEN','MAROON','YELLOW GREEN']
        # graph run flag
        self.grpRun = False


    def SetGraphRange(self, count):
        # create a new set of data
        if self.index is None:
            self.index = [x for x in range(0,count)]
            self.value = [[0]*count,[0]*count]
        # modify existing
        else:
            old_count = len(self.index)
            self.index = [x for x in range(0,count)]
            # decrease size
            if count < old_count:
                for idx in range(len(self.value)):
                    self.value[idx] = self.value[idx][old_count-count:]
            # increase size
            else:
                for idx in range(len(self.value)):
                    self.value[idx] = [0] * (count-old_count) + self.value[idx]


    def OnNewDataSize(self, evt):
        self.SetGraphRange(int(self.choDSize.GetString(
            self.choDSize.GetCurrentSelection())))


    def OnGraphRun(self, evt):
        if self.tglRun.GetValue():
            self.grpRun = True
            self.tglRun.SetLabel('PAUSE')
        else:
            self.grpRun = False
            self.tglRun.SetLabel('RUN')
            self.ComputeStats()

    def ComputeStats(self):
        self.lscStats.ClearAll()
        self.lscStats.InsertColumn(0,'Item',width=120)
        self.lscStats.InsertColumn(1, 'Value')
        for idx in range(len(self.value)):
            self.lscStats.InsertItem(5*idx,'Touch {:d} max'.format(idx))
            self.lscStats.SetItem(5*idx,1,
                    '{:d}'.format(max(self.value[idx])))
            self.lscStats.InsertItem(5*idx+1,'Touch {:d} min'.format(idx))
            self.lscStats.SetItem(5*idx+1,1,
                    '{:d}'.format(min(self.value[idx])))
            self.lscStats.InsertItem(5*idx+2,'Touch {:d} mean'.format(idx))
            self.lscStats.SetItem(5*idx+2,1,
                    '{:.1f}'.format(st.mean(self.value[idx])))
            self.lscStats.InsertItem(5*idx+3,'Touch {:d} stdev'.format(idx))
            self.lscStats.SetItem(5*idx+3,1,
                    '{:.2f}'.format(st.stdev(self.value[idx])))
            self.lscStats.InsertItem(5*idx+4,'')



    def OnUpdateComData(self, evt):
        for byte in evt.data:
            # feed incoming byte to the packet decoder
            ret = self.pd.AddByte(byte)
            if ret and ret[0] == RPT_U16XXX:
                idx = 0
                lines = []
                while len(ret) > (idx * 2 + 1):
                    # convert data into u16 and append it to the value
                    self.value[idx].append((ret[idx*2+1]<<8) + ret[idx*2+2])
                    self.value[idx].pop(0)
                    # create lines
                    lines.append(wxplot.PolyLine(
                        list(zip(self.index, self.value[idx])),
                        colour=self.grpColor[idx], width=2,
                        style=wx.PENSTYLE_SOLID))

                    # advance index
                    idx += 1

                # refresh graph
                if self.grpRun:
                    graphics = wxplot.PlotGraphics(lines)
                    self.grpTouch.Draw(graphics)

#
#--------1---------2---------3---------4---------5---------6---------7---------8
##
# \brief Main frame
#
class MyFrame(wx.Frame):

    def __init__(self, parent, title):
        wx.Frame.__init__(self, parent, title=title)
        frameSize = (900,600)

        # notebook panel
        self.pnlBook = wx.Notebook(self)
        # serial terminal panel
        self.pnlTerm = TermPanel(self.pnlBook, serial.Serial(), size=frameSize)
        # plot panel
        self.pnlPlot = GraphPanel(self.pnlBook, size = frameSize)

        self.pnlBook.InsertPage(0,self.pnlTerm,'Terminal')
        self.pnlBook.InsertPage(1,self.pnlPlot,'Graph')

        # event handler
        self.pnlBook.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGED, self.OnPageChanged)
        self.Bind(wx.EVT_CLOSE, self.OnClose)

        # variables
        self.close_flag = False

        # sizer
        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(self.pnlBook, 1, wx.EXPAND)

        self.SetSizer(self.sizer)
        self.SetAutoLayout(1)
        self.sizer.Fit(self)

        self.Show()

    def OnPageChanged(self, evt):
        # workaround wx late message issue
        if self.close_flag:
            return

        if self.pnlBook.GetSelection() == 0:
            self.pnlTerm.thread.SetEventTarget(self.pnlTerm)

        elif self.pnlBook.GetSelection() == 1:
            self.pnlTerm.thread.SetEventTarget(self.pnlPlot)

    def OnClose(self, evt):
        # set the close flag first
        self.close_flag = True
        # then destroy
        self.Destroy()


#--------1---------2---------3---------4---------5---------6---------7---------8
if __name__=="__main__":
    # app loop
    app = wx.App()
    frame = MyFrame(None, "Touch Sensor Monitor")
    app.MainLoop()
