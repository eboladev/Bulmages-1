# -*- coding: utf-8 -*-
# svimage.py from rtf2pdf at http://rst2pdf.googlecode.com
# by Roberto Alsina <ralsina@kde.org>
# downloaded by xdrudis for bulmages from 
# http://code.google.com/p/rst2pdf/source/browse/trunk/rst2pdf/svgimage.py
# under the MIT license:
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

# modified by xdrudis to avoid ending the page (replacing a noop showPage())
# without modifying uniconvertor, and making uniconvertor presence detection
# work for me, work around lack of font support for files without text, and avoid unisave  
import sys,os
from reportlab.platypus import *

HAS_UNICONVERTOR=False

class DummyOut:
   def write( m):
      pass
 
sortida=sys.stdout
xungo=sys.exit
try:
    sys.stdout=DummyOut()
    sys.exit=lambda x:1 
    import uniconvertor
    sys.path.insert(1, os.path.dirname( uniconvertor.__file__))
    from app.io import load
    from app.plugins import plugins
    import app

    app.init_lib()
    plugins.load_plugin_configuration()
    HAS_UNICONVERTOR=True

## workaround for a bug that stops bgtrml2pdf from working with svg
## files winth font properties in style attributes, even if they apply
## to no text because all text is empty or transformed to paths
## see http://svn.berlios.de/viewcvs/bulmages/branches/docsMonolitic/bulmages/installbulmages/openreports/ca/

    class FakeFont(app.Graphics.font.Font):
      def __init__(self, name):
        self.name = name
        self.family = name
        self.font_attrs = 'Regular Italic'
        self.xlfd_start = '-adobe-Nimbus Roman No9 L-regular-i-normal'
        self.encoding_name = 'iso8859-1'
        self.outlines = None
        self.ref_count = 0
        app.Graphics.font.font_cache[self.name] = self

    def ensure_there_is_at_least_a_fake_fallback_font():
       fallback = 'Slim';
       if (not (app.config is  None)) and (hasattr(app.config,'preferences'))  and (hasattr(app.config.preferences,'fallback_font')):
               fallback = app.config.preferences.fallback_font
       if not app.Graphics.font.font_cache.has_key(fallback):
              return FakeFont(fallback)
       return app.Graphics.font.font_cache[fallback]
    
    
    fallbackFont = ensure_there_is_at_least_a_fake_fallback_font()


except :
    pass

sys.exit=xungo
sys.stdout=sortida

## end of font workaround


class SVGImage(Flowable):
    def __init__(self,filename,width=None,height=None):
        Flowable.__init__(self)
        if HAS_UNICONVERTOR:
            self.doc = load.load_drawing(filename)
            self.saver = plugins.find_export_plugin(plugins.guess_export_plugin(".pdf"))
            self.width=width
            self.height=height
            _,_,self._w,self._h=self.doc.BoundingRect()
            if not self.width: self.width=self._w
            if not self.height: self.height=self._h
        else:
            raise ImportError("SVG image support not enabled, install uniconvertor >= 1.1.3")
   
    def wrap(self,aW,aH):
        if HAS_UNICONVERTOR:
            return self.width ,self.height
        return 0,0
   
    def drawOn(self,canv,x,y,_sW=0):
        if HAS_UNICONVERTOR:

            if _sW and hasattr(self,'hAlign'):
                from reportlab.lib.enums import TA_LEFT, TA_CENTER, TA_RIGHT, TA_JUSTIFY
                a = self.hAlign
                if a in ('CENTER','CENTRE', TA_CENTER):
                    x = x + 0.5*_sW
                elif a in ('RIGHT',TA_RIGHT):
                    x = x + _sW
                elif a not in ('LEFT',TA_LEFT):
                    raise ValueError, "Bad hAlign value "+str(a)
       
            canv.saveState()
            canv.translate(x,y)
            canv.scale(self.width/self._w,self.height/self._h)
            sp = canv.showPage
            canv.showPage=(lambda : 1) 
            self.saver (self.doc,open(".ignoreme.pdf","w"),".ignoreme.pdf",options={'pdfgen_canvas':canv})
            os.unlink(".ignoreme.pdf")
            canv.showPage=sp
            canv.restoreState()
           
           
if __name__ == "__main__":
    from reportlab.lib.styles import getSampleStyleSheet
    doc = SimpleDocTemplate("svgtest.pdf")
    styles = getSampleStyleSheet()
    style= styles["Normal"]
    Story = [Paragraph("Before the image",style),
             SVGImage(sys.argv[1]),
             Paragraph("After the image",style)]
    doc.build(Story)
   

