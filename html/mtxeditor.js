/*
 *******************************************************************************
 ** Created by Manuel Schreiner
 **
 ** Copyright © 2022 io-expert.com. All rights reserved.
 **
 ** 1. Redistributions of source code must retain the above copyright notice,
 **    this condition and the following disclaimer.
 **
 ** This software is provided by the copyright holder and contributors "AS IS"
 ** and any warranties related to this software are DISCLAIMED.
 ** The copyright owner or contributors be NOT LIABLE for any damages caused
 ** by use of this software.

 *******************************************************************************
 */

 const layoutModes = {
    leftToRightZickZack: 0,
    rightToLeftZickZack: 1,
    leftToRight: 2,
    rightToLeft: 3,
  };

class MatrixEditor {
    static get layoutModes() {
        return layoutModes;
    }

    /**
     * 
     * @param {*} height in pixels of matrix module
     * @param {*} width in pixels of matrix module 
     * @param {*} canvasid optional: id of canvas object to work with
     * @param {*} layout will set the layoutMode 
     */
    constructor(height=8, width=8, canvasid="matrixcanvas", layout=layoutModes.leftToRightZickZack) {
        this.height = height;
        this.width = width;
        this.redraw = false;
        this.pixels = [];
        this.layout = layout;
        this.onpixelchange = undefined;
        this.onactivecolorchange = undefined;
        this.drawLeds = false;

        this.activeColor = "#FFFFFF";

        this.pixel = {
            size:10,
            padding:2
        };

        this.canvas = {
            handle:undefined,
            id:canvasid,
            offset:{
                x:0,
                y:0
            }
        };

        // set all pixels to black
        for(var x = 0; x < this.width;x++)
        {
            this.pixels[x] = [];
            for(var y = 0; y < this.height;y++)
            {
                this.pixels[x][y] = "#000000"
            }
        }

        this.reinitCanvas(this.canvas.id);

        window.setInterval(() => {
            if (this.redraw)
            {
                this.redraw = false;
                this.draw();
            }
        },200);
    }

    /**
     * Set new active color
     * 
     * @param {*} color new color
     */
    setColor(color)
    {
        this.activeColor = color;
        if ((self.onactivecolorchange !== undefined) && (self.onactivecolorchange !== null) && (typeof self.onactivecolorchange !== undefined))
        {
            let callbackevent = {
                color: self.activeColor
            };
            self.onactivecolorchange(callbackevent);
        }
    }

    /**
     * Update all variables which could change by a window resize
     */
    updateVars()
    {
        let self = this;
        self.canvas.offset.x = self.canvas.handle.getBoundingClientRect().left;
        self.canvas.offset.y = self.canvas.handle.getBoundingClientRect().top;
        let dx = self.canvas.handle.width / self.width;
        let dy = self.canvas.handle.height / self.height;
        self.pixel.size = Math.min(dx,dy);
    }

    /**
     * Update cursor dependend if in draw area or not
     * 
     * @param {*} self self / this class
     * @param {*} event event which is called by mousemove
     */
    updateCursor(self,event)
    {
        self.updateVars();
        let movePos = {
            x:event.clientX,
            y:event.clientY
        };
        
        let canvasMovePos = {
            x:(movePos.x - self.canvas.offset.x),
            y:(movePos.y - self.canvas.offset.y)
        };

        let elementPos = {
            x:Math.floor(canvasMovePos.x / self.pixel.size),
            y:Math.floor(canvasMovePos.y / self.pixel.size)
        };

        if ((elementPos.x >= self.width) || (elementPos.y >= self.height))
        {
            self.canvas.handle.style.cursor = "auto";
        } else
        {
            self.canvas.handle.style.cursor = "crosshair";
        }
    }

    /**
     * Click / Mousedown move through a pixel
     * 
     * @param {*} self self / this class
     * @param {*} event event which is called by mousemove / click
     */
    pixelClicked(self,event)
    {
        self.updateVars();
        let clickPos = {
            x:event.clientX,
            y:event.clientY
        };
        
        let canvasClickPos = {
            x:(clickPos.x - self.canvas.offset.x),
            y:(clickPos.y - self.canvas.offset.y)
        };

        let elementPos = {
            x:Math.floor(canvasClickPos.x / self.pixel.size),
            y:Math.floor(canvasClickPos.y / self.pixel.size)
        };

        if (self.pixels[elementPos.x][elementPos.y] != self.activeColor)
        {
            self.pixels[elementPos.x][elementPos.y] = self.activeColor;
            if ((self.onpixelchange !== undefined)  && (self.onpixelchange !== null) && (typeof self.onpixelchange !== undefined))
            {
                let callbackevent = {
                    position: {
                        x:elementPos.x,
                        y:elementPos.y,
                    },
                    color: self.activeColor
                };
                self.onpixelchange(callbackevent);
            }
            self.forceRedraw();
        };
    }

    /**
     * Execute as soon class was loaded
     */
    classLoaded()
    {
        this.draw();
        var self = this;

        self.updateVars();

        this.canvas.handle.addEventListener('mousedown', function(event) {
            self.pixelClicked(self,event);
        });

        this.canvas.handle.addEventListener('mousemove', function(event) {
            if(event.buttons == 1) {
               event.preventDefault();
               self.pixelClicked(self,event);
            }
            self.updateCursor(self,event)
        });

        this.canvas.handle.addEventListener('touchstart', function(event) {
            if(event.touches.length == 1) {
                event.preventDefault();
                self.pixelClicked(self,event.touches[0]);
            }
        });
        
        this.canvas.handle.addEventListener('touchmove', function(event) {
            if(event.touches.length == 1) {
                event.preventDefault();
                self.pixelClicked(self,event.touches[0]);
            }
        });
        
    }

    /**
     * Check class is loaded...
     */
    checkLoaded()
    {
        let self = this;

        if(typeof this.canvas.handle == 'undefined' || self.canvas.handle == null)
        {
            self.canvas.handle = document.getElementById(self.canvas.id);
            if(typeof this.canvas.handle == 'undefined' || self.canvas.handle == null)
            {
                this.canvas.handle = undefined;
            } else
            {
                this.classLoaded();
            }
        }
        
    }

    /**
     * Reinitialize canvas
     * 
     * @param {*} canvasName new canvas name
     */
    reinitCanvas(canvasName)
    {
        let self = this;

        self.canvas.id = canvasName;
        self.canvas.handle = undefined;
        let loadingInterval = window.setInterval(() => {
            self.checkLoaded();
            if(typeof self.canvas.handle !== 'undefined' && self.canvas.handle !== null)
            {
                window.clearInterval(loadingInterval);
                console.log("Canvas Initialized");
            }
        },500);
    }

    /**
     * Create drawing context
     * 
     * @returns context
     */
    createDrawContext()
    {
        var ctx = this.canvas.handle.getContext("2d");
        return ctx;
    }

    drawLed(context,x,y,width,height)
    {
        context.beginPath();
        context.ellipse(x + width/2, y + height/2, width/2-2, height/2-2, 0, 0, 2 * Math.PI);
        context.fill();
    }
    /**
     * Draw Matrix
     */
    drawMatrix()
    {
        let self = this;
        self.updateVars();
        var ctx = this.createDrawContext();
        for(var y = 0; y < this.height;y++)
        {
            for(var x = 0; x < this.width;x++)
            {
                ctx.fillStyle = self.pixels[x][y];
                if ((self.pixels[x][y] === "#000000") || (self.pixels[x][y] === "black") || (self.drawLeds === false))
                {
                    ctx.fillRect(x*(self.pixel.size),y*(self.pixel.size),self.pixel.size-self.pixel.padding,self.pixel.size-self.pixel.padding);
                } else
                {
                    self.drawLed(ctx,x*(self.pixel.size),y*(self.pixel.size),self.pixel.size-self.pixel.padding,self.pixel.size-self.pixel.padding);
                }
            }
        }
    }

    /**
     * Force redraw
     */
    forceRedraw()
    {
        //this.redraw = true; //optionally async redraw by seperate interval event
        this.draw();
    }

    /**
     * Draw canvas
     */
    draw()
    {
        this.drawMatrix();
    }

    /**
     * Clear canvas
     */
    clear()
    {
        var ctx = this.createDrawContext();
        ctx.fillStyle = "#000000";
        ctx.clearRect(0,0,this.canvas.handle.width,this.canvas.handle.height);
        for(var x = 0; x < this.width;x++)
        {
            this.pixels[x] = [];
            for(var y = 0; y < this.height;y++)
            {
                this.pixels[x][y] = "#000000"
            }
        }
        this.forceRedraw();
    }

    setNewSize(width,height)
    {
        this.width = width;
        this.height = height;
        // set all pixels to black
        this.pixels = [];
        for(var x = 0; x < this.width;x++)
        {
            this.pixels[x] = [];
            for(var y = 0; y < this.height;y++)
            {
                this.pixels[x][y] = "#000000"
            }
        }
        this.clear();
    }

    /**
     * Convert hex color code to RGB
     * 
     * @param {*} hex hex color code
     * @returns 
     */
    hexToRGB(hex) {
        let r = 0;
        let g = 0;
        let b = 0;
      
        if (hex.length == 7) {
          r = "0x" + hex[1] + hex[2];
          g = "0x" + hex[3] + hex[4];
          b = "0x" + hex[5] + hex[6];
        }
        
        return {r:r,g:g,b:b};
    }

    /**
     * Download CPP file
     * 
     * @param {*} filename name of CPP file, example: image.cpp 
     */
    download(filename) {
        var downloadElement = document.createElement('a');
        var text = "/* automatically created by IO-Expert Arduino-LED-Matrix-Editor */\r\n"
        text += "#include \"FastLED.h\"\r\n";
        text += "CRGB image[" + (this.width * this.height) + "] = {\r\n";
        for(var y = 0; y < this.height;y++)
        {
            for(var x = 0; x < this.width;x++)
            {
                var rgb = this.hexToRGB(this.pixels[x][y])
                if ((this.layout == layoutModes.leftToRightZickZack) && (y % 2 == 0))
                {
                    rgb = this.hexToRGB(this.pixels[this.width-x-1][y])
                } else if ((this.layout == layoutModes.rightToLeftZickZack) && (y % 2 == 1))
                {
                    rgb = this.hexToRGB(this.pixels[this.width-x-1][y])
                } else if (this.layout == layoutModes.rightToLeft)
                {
                    rgb = this.hexToRGB(this.pixels[this.width-x-1][y])
                }
                text += "CRGB(" + rgb.r + "," + rgb.g + "," + rgb.b + "), "
            }
            text += "\r\n"
        }
        text += "};\r\n"

        downloadElement.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(text));
        downloadElement.setAttribute('download', filename);
      
        downloadElement.style.display = 'none';
        document.body.appendChild(downloadElement);
      
        downloadElement.click();
      
        document.body.removeChild(downloadElement);
      }

      rgbToHex(r, g, b) {
        return ((r << 16) | (g << 8) | b).toString(16);
      }

      reinitPixelDataFromImage(self)
      {
         var payload = {
             i: 'c'
         };
         var data = new FormData();
         data.append( "json", JSON.stringify( payload ) );
         txQueue.enqueue(data);

         for(var y = 0;y < self.height; y++)
         {
            for(var x = 0;x < self.width; x++)
            {
                self.pixels[x][y] = self.getColorOfPixel(self,x,y);
                let callbackevent = {
                    position: {
                        x:x,
                        y:y,
                    },
                    color: self.pixels[x][y]
                };
                if (self.pixels[x][y] !== "#000000")
                {
                    self.onpixelchange(callbackevent);
                }
            }
         }
         ctx.clearRect(0,0,self.canvas.handle.width,self.canvas.handle.height);
         self.draw();
      }

      getColorOfPixel(self,x,y)
      {
        var ctx = self.createDrawContext();
        var offset = {
            x:x*self.pixel.size,
            y:y*self.pixel.size
        };
        var r = 0;
        var g = 0;
        var b = 0;
        var count = 0;
        for (var yi = 2;yi < self.pixel.size - 2;yi++)
        {
            for (var xi = 2;xi < self.pixel.size - 2;xi++)
            {
                let colordata = ctx.getImageData(offset.x + xi, offset.y + yi, 1, 1).data
                r = r + colordata[0];
                g = g + colordata[1];
                b = b + colordata[2];
                count++;
            }
        }
        r = Math.floor(r / (count));
        g = Math.floor(g / (count));
        b = Math.floor(b / (count));
        let hex = "#" + ("000000" + self.rgbToHex(r,g,b)).slice(-6);
        return hex;
      }

      loadImage(img)
      {
        var img1 = new Image();
        var ctx = this.createDrawContext();

        //drawing of the test image - img1
        let self = this;
        img1.onload = function () {
            ctx.clearRect(0,0,self.canvas.handle.width,self.canvas.handle.height);
            ctx.drawImage(img1, 0, 0, self.width*self.pixel.size, self.height*self.pixel.size);
            self.reinitPixelDataFromImage(self);
        };
        img1.src = img;
      }
}

if (typeof module != "undefined")
{
    module.exports.default = module.exports = {MatrixEditor}
}


