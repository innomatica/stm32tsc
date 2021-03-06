#!/usr/bin/env python3

################################################################################
#
#   \file
#   \author     <a href="http://www.innomatic.ca">innomatic</a>
#   \brief      Wxplot example
#   \copyright  <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons Licence" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.
#

import wx
from wx.lib import plot as wxplot
from wx.lib.embeddedimage import PyEmbeddedImage

# following embedded images are generated by img2py()
peiHome = PyEmbeddedImage(
    b'iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAAABHNCSVQICAgIfAhkiAAAAAlw'
    b'SFlzAAADsAAAA7ABJ8QPrQAAAXZJREFUSInt1LFLllEUBvDfSSPFJTfHlpoih1YhSjLaam/L'
    b'ocgG+xMaXBoKarP+ASF0iZbmmgKnaq4hKS3bTMvT4BVu76ff91oKCR048N5zOM/z3HPOeyMz'
    b'HaQdOVD0/wSHiyAiLkTEx4j4HBGXq/iHiMiI2IyIiY7CzOzpuIENZPEfmC65L1X8SkdtD+A+'
    b'PKwAHmCmOs9itRtB7PajRcRxzOFiUX8L3zCATTzGsUbZ1cxc6NkinMS7omoZ53G3UnoPY1iq'
    b'Yu1ahPGqr29wBk8bQIlnOI3FKjbVlQA3q2E+L+CLO4BnJWAU8+X8E3c6CGyt66Oq8D7O4VMX'
    b'8G1fKXOqh/8E/TXBcOnnOiZxHd9bgG/7BqZwDWulxSO/bVFEnMVgueZLf2aXbK1tX2a+wq5D'
    b'bqu86R1b1L8HdbN4Ub7HcLtN0V4IXmfmHETE0bYE/85rup8Eq3+B97UZ2PGxi4hTGGqE32fm'
    b'SskP40Qjv5aZb1sR7Kf9ArsbUbWu3OyGAAAAAElFTkSuQmCC')

peiMove = PyEmbeddedImage(
    b'iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAAABHNCSVQICAgIfAhkiAAAAAlw'
    b'SFlzAAADsAAAA7ABJ8QPrQAAAYNJREFUSIm1lTtLA0EUhb8TggqBEEJqLewFtRFB8JF/oNhZ'
    b'iV2wM621ttrYa6OFRbooFkYrrfQPCIKViBAVFORaOOo4yW52Vzxw2d37OGd25s6MzIz/RC5N'
    b'sqQlScupFMwskQE1wJzVE9dlIE8lkpU8sYjiFlmSgDZQiEh5A4pm9hrFEbvI9qk+CkwDDS90'
    b'5HwjceRfJEnXYYefqdlNWtfxB5ImJRVjR9UFkgqSpkJ/LkiqAefAWloBYBU4lVTvKuDIt9zn'
    b'QAaBfvfc+CUS0YpPwF1gz178pUu8TZcWBpggus//ajMAReDiH8ivgLLMDEkl4BgYdzPXAPaC'
    b'OV4B5tz7GbAdxBecAVwDs2Z27/d5Cbh06ptp9wGw7o280rEPzOwRqAIHQJP0OAEOv0fukPcz'
    b'nMhiBnLMrAW0Qn+qCycLYk9TAEn7wDAwCFSc+wG4AW6BeTN7j6rPRwU8DAFjga/srHd9glPU'
    b'766wzys96xMe1aFIIvKeN5oPtxmbQB9Q9VsxDh9ePiFv5GfibAAAAABJRU5ErkJggg==')

peiZoom = PyEmbeddedImage(
    b'iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAAABHNCSVQICAgIfAhkiAAAAAlw'
    b'SFlzAAADsAAAA7ABJ8QPrQAAAbRJREFUSIm1lb1KXFEQx3//KCjYabFgYZNYGGwVKwVFQiCQ'
    b'IoUSiE3U9HaCKL6ApEiTB4iNhcFHELRJHkAjSsCkSJQICpqFXcdi74W5Z+/HCdGBgcvc/8fe'
    b'OXNmZWY8ZDx6UPUyA0ldkuYk7Uo6lfRH0ndJO5KeS1KUg5m1JbAM/AasJL8Bs3n8jFaO+IcK'
    b'YZ+3wEK0QY54E/gMvAFeAovA3r+YePHJgHgIPC5o4Thw4bB1oFZlsO0Ix0B/6afDCHDlOKuF'
    b'BsAA0HDguarDS3jvHecn0FlksOCA50BXpMFg0NaREJPegz43uQdmVi8aax9mdgTcuFJviEkN'
    b'LKcWGyp4zoidudpTST1RytIw0O1Kp22gpJc1WqOW9vJd5Bl8dJyTqjH9RHYinlSITwF/HWej'
    b'ymCM7ET8IGcqkj6/Aq4d9pKCexOS1wITA/aBJWAeWKF1w0PMNTAdu+zyTGIy16Sov28Lfmm4'
    b'q9bJboA2k7JDFPAM2AK+AgfAF2ADmAA6EtzrMpPKUYwc19DkBhj1q+K/wsw2af1nNJNSN/Ai'
    b'fXlvCczQuhu/gCEzQ8mLe4tkzTTShXkHu78AJI6s+WwAAAAASUVORK5CYII=')

peiSave = PyEmbeddedImage(
    b'iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAAABHNCSVQICAgIfAhkiAAAAAlw'
    b'SFlzAAADsAAAA7ABJ8QPrQAAAWxJREFUSInVlr1KA0EUhb8bgrERBMEylRbRBwgWggbtbMRC'
    b'UKt0qVJYamvjO9jZCIKK4gvYCVZBtA1apBIUlYA/xyKzuK6z2WRxAx4Y5nL3zjlz7/ytSSJL'
    b'5DJlB/KBYWY5oAwM9TG+DVxJeo+NkIQr0w6gFO0OmAt4oi1comngBagCB565HLpvVWDf+fZc'
    b'xudmVknK4Bh4cPamZ6a1UOy68y0AJaAFvAKVbhmkgqQbYB54As6imfzJLvKITKURsBg7LLII'
    b'FIB64M9HAx0+PL4tM1tzdtEXJ6lhZvfARJLAEbAKjEX8465vA6fApWesCGXoFZDUBGZixPtC'
    b'5lfF4O6iMMxsEjjh9xrE4RFYkdToSQBYonNCL4DnBPJhOmdgGehZIChdTdJ1N3YzKwJNYsr9'
    b'/xd5oLtIQMHM6sCs822YWSuBY9T1ZTd2xHE51u87fpt0L5qv7Qa8Fv6rMLMS/b3JPrwBt5I+'
    b'gZ8CWeALA9y6UzVKmtYAAAAASUVORK5CYII=')


#--------1---------2---------3---------4---------5---------6---------7---------8
## wx.lib.wxplot example on wx.Panel
#
# Create and expose wx.lib.wxplot canvas object on wx.Panel
#
class WplGraph(wx.Panel):

    def __init__(self, *args, **kwgs):

        wx.Panel.__init__(self, *args, **kwgs)

        # canvas object
        self.canvas = wxplot.PlotCanvas(self)
        # toolbar id
        self.tidHome = wx.NewId()
        self.tidDrag = wx.NewId()
        self.tidZoom = wx.NewId()
        self.tidSave = wx.NewId()
        # toolbar
        self.toolbar = wx.ToolBar(self)
        # size
        self.toolbar.SetToolBitmapSize((24,24))
        # toolbar buttons
        self.toolbar.AddTool(self.tidHome, '', peiHome.GetBitmap(),
                shortHelp = 'Home')
        self.toolbar.AddCheckTool(self.tidDrag, '', peiMove.GetBitmap(),
                shortHelp = 'Pan')
        self.toolbar.AddCheckTool(self.tidZoom, '', peiZoom.GetBitmap(),
                shortHelp = 'Zoom')
        self.toolbar.AddTool(self.tidSave, '', peiSave.GetBitmap(),
                shortHelp = 'Save')
        self.toolbar.Realize()

        self.Bind(wx.EVT_TOOL, self.OnToolClick, id=self.tidHome)
        self.Bind(wx.EVT_TOOL, self.OnToolClick, id=self.tidDrag)
        self.Bind(wx.EVT_TOOL, self.OnToolClick, id=self.tidZoom)
        self.Bind(wx.EVT_TOOL, self.OnToolClick, id=self.tidSave)

        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(self.canvas, 1, wx.LEFT | wx.TOP | wx.EXPAND)
        self.sizer.Add(self.toolbar, 0, wx.LEFT | wx.EXPAND)
        self.SetSizer(self.sizer)
        # do not call this as a child
        #sizer_h.Fit(self)

    ## handles mouse click on toolbar
    def OnToolClick(self, evt):
        tid = evt.GetId()

        if tid == self.tidHome:
            self.canvas.Reset()

        elif tid == self.tidDrag:
            if self.toolbar.GetToolState(self.tidDrag):
                self.canvas.enableDrag = True
                self.toolbar.EnableTool(self.tidHome, False)
                self.toolbar.EnableTool(self.tidZoom, False)
                self.toolbar.EnableTool(self.tidSave, False)
            else:
                self.canvas.enableDrag = False
                self.toolbar.EnableTool(self.tidHome, True)
                self.toolbar.EnableTool(self.tidZoom, True)
                self.toolbar.EnableTool(self.tidSave, True)

        elif tid == self.tidZoom:
            if self.toolbar.GetToolState(self.tidZoom):
                self.canvas.enableZoom = True
                self.toolbar.EnableTool(self.tidHome, False)
                self.toolbar.EnableTool(self.tidDrag, False)
                self.toolbar.EnableTool(self.tidSave, False)
            else:
                self.canvas.enableZoom = False
                self.toolbar.EnableTool(self.tidHome, True)
                self.toolbar.EnableTool(self.tidDrag, True)
                self.toolbar.EnableTool(self.tidSave, True)

        elif tid == self.tidSave: 
            with wx.FileDialog(self, "Save plot file",
                    wildcard="image files (*.jpg)|*.jpg",
                    style=wx.FD_SAVE | wx.FD_OVERWRITE_PROMPT) as fileDialog:

                if fileDialog.ShowModal() == wx.ID_OK:
                    print('file path', fileDialog.GetPath())

                    self.canvas.SaveFile(fileDialog.GetPath())

    ## return canvas object
    def GetCanvas(self):
        return self.canvas

    ## plot wxplot.Graphics object
    def Draw(self, graphics):
        self.canvas.Draw(graphics)

    ## set canvas axes pen
    def SetPen(self, pen):
        self.canvas.axesPen = pen

    ## clear canvas
    def Clear(self):
        self.canvas.Clear()


#--------1---------2---------3---------4---------5---------6---------7---------8
if __name__=="__main__":

    import numpy as np

    class MyFrame(wx.Frame):

        def __init__(self, parent, title):
            wx.Frame.__init__(self, parent, title=title)

            # menu id
            self.idLine = wx.NewId()
            self.idHist= wx.NewId()
            # menu
            menu = wx.Menu()
            # menu item
            mitemLine = menu.Append(self.idLine, "Line", "")
            mitemHist = menu.Append(self.idHist, "Histogram", "")

            # menubar
            menubar = wx.MenuBar()
            menubar.Append(menu, "Demo")
            self.SetMenuBar(menubar)

            # panel
            self.pnlPlot = WplGraph(self, size=(640,480))

            # event binding
            self.Bind(wx.EVT_MENU, self.OnDrawGraph, mitemLine)
            self.Bind(wx.EVT_MENU, self.OnDrawGraph, mitemHist)
            self.Bind(wx.EVT_CLOSE, self.OnClose)

            # sizer
            self.sizer = wx.BoxSizer(wx.VERTICAL)
            self.sizer.Add(self.pnlPlot,1, wx.EXPAND)

            self.SetSizer(self.sizer)
            self.SetAutoLayout(1)
            self.sizer.Fit(self)
            self.Show()


        def OnDrawGraph(self, evt):

            # simple line plot
            if evt.GetId() == self.idLine:
                # clear previous plot
                self.pnlPlot.Clear()

                x = np.linspace(0,10,500)
                y = np.sin(x)

                # create lines
                line1 = wxplot.PolyLine(list(zip(x, np.sin(x))),
                        colour='red', width=3, style=wx.PENSTYLE_DOT_DASH)
                line2 = wxplot.PolyLine(list(zip(x, -np.sin(x))),
                        colour='blue', width=3, style=wx.PENSTYLE_LONG_DASH)

                # create a graphics
                graphics = wxplot.PlotGraphics([line1, line2])
                self.pnlPlot.Draw(graphics)

            # histogram
            elif evt.GetId() == self.idHist:
                # clear previous plot
                self.pnlPlot.Clear()

                np.random.seed(0)
                # fixed bins on the right
                x1 = np.random.normal(400, 25, size=100)
                h1,b1 = np.histogram(x1, bins=8)
                hist1 = wxplot.PolyHistogram(h1,b1,fillcolour='red')
                # variable bins on the left
                x2 = np.random.normal(200, 25, size=100)
                h2,b2 = np.histogram(x2,
                        bins=[100, 150, 180, 195, 205, 220, 250, 300])
                hist2 = wxplot.PolyHistogram(h2, b2,fillcolour='blue')

                # graph title and axis titles
                graphics = wxplot.PlotGraphics([hist1, hist2],
                        'Histogram with variable binsize and fixed binsize',
                        'value', 'count')
                self.pnlPlot.Draw(graphics)

        def OnClose(self, evt):
            # explicitly destroy the panel
            self.pnlPlot.Close()
            # destroy self
            self.Destroy()


    app = wx.App()
    frame = MyFrame(None, "WplGraph demo")

    app.MainLoop()
