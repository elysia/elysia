    /*
      Missile Fleet - a real-time tactics game / browser benchmark

      Copyright (C) 2007  Ilmari Heikkinen

      This program is free software; you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation; either version 3 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program; if not, write to the Free Software
      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

      http://www.gnu.org/copyleft/gpl.html
    */
    windowWidth=1024
    windowHeight=768

    DEFAULT_CURSOR = 'default'
    MOVE_TO_CURSOR = 'url(moveto.png) 9 9, move'
    TARGET_CURSOR = 'crosshair'
    SELECT_CURSOR = 'pointer'

    M = {
      rotation : function(rotation) {
        return CanvasSupport.tRotationMatrix(rotation)
      },

      scaling : function(x, y) {
        return CanvasSupport.tScalingMatrix(x, y)
      },

      translation : function(x, y) {
        return CanvasSupport.tTranslationMatrix(x, y)
      }
    }

    V = {
      rotate : function(v, rotation) {
        return V.multiply(v, M.rotation(rotation))
      },

      add : function(v, u) {
        return [u[0] + v[0], u[1] + v[1]]
      },

      multiply : function(v, matrix) {
        return CanvasSupport.tMatrixMultiplyPoint(matrix, v[0], v[1])
      }
    }

    Context = function() {
        context={};
        context.selection=[];
        context.toggleSelect = function(s) {
            if (this.selection.indexOf(s.id) == -1)
                this.select(s);
            else
                this.deselect(s);
        };
        context.clearSelection = function() {
            var dict = this.selection.dict
            while (this.selection.length > 0) {
                this.deselect(this.selection.dict[this.selection[0]])
            }
        };
        context.select = function(s) {
            if (!this.selection.dict) this.selection.dict = {}
            s.root.dispatchEvent({type : 'select', canvasTarget: s})
            this.selection.dict[s.id] = s
            this.selection.push(s.id)
        };
        context.deselect = function(s) {
            if (!this.selection.dict) this.selection.dict = {}
            s.root.dispatchEvent({type : 'deselect', canvasTarget: s})
            delete this.selection.dict[s.id]
            this.selection.deleteFirst(s.id)
        };
        context.getSelectionCenter = function() {
            var x = 0
            var y = 0
            for(var i=0; i<this.selection.length; i++) {
                var s = this.selection.dict[this.selection[i]]
                x += s.x
                y += s.y
            }
            return [ x / this.selection.length, y / this.selection.length ]
        };
        context.setLobeTarget= function(point)  {
            
        };
        return context;
    }
  
    currentMaxZ = 2;

    Lobe = Klass(CanvasNode, {
            initialize: function() {
                CanvasNode.initialize.call(this);
                this.lobe=new Rectangle(64,64, {
                        stroke : false,
                        strokeOpacity : 0.1,
                        stroke : '#ffff00',
                        fillOpacity : 0.1,
                        fill : '#ffff00',
                        visible : true,
                        zIndex : currentMaxZ+=2
                    });
                this.append(this.lobe)
            }
        });
      

    Editor = Klass(CanvasNode, {
      bgColor : 'rgb(0,0,0)',
      bgOpacity : 0.15,


      initialize : function() {
        CanvasNode.initialize.call(this)
        this.context=Context();
        this.bg = new Rectangle(this.width, this.height)
        this.bg.fill = this.bgColor
        this.bg.fillOpacity = this.bgOpacity
        var selectionStart, startX, startY
        var th = this
        var objectsInside = function(rect) {
          return th.childNodes.filter(function(s) {
            var x1 = Math.min(rect.cx, rect.x2)
            var x2 = Math.max(rect.cx, rect.x2)
            var y1 = Math.min(rect.cy, rect.y2)
            var y2 = Math.max(rect.cy, rect.y2)
            return (s.x >= x1 && s.x <= x2 && s.y >= y1 && s.y <= y2)
          })
        }
        this.selectRect = new Rectangle(0,0, {
          stroke : 1,
          strokeOpacity : 0.4,
          stroke : '#00ff00',
          fillOpacity : 0.1,
          fill : '#00ff00',
          visible : false,
          zIndex : 999
        })
        this.append(this.selectRect)
        this.bg.addEventListener('mousedown', function(ev) {
          ev.preventDefault()
          var point = CanvasSupport.tMatrixMultiplyPoint(
            CanvasSupport.tInvertMatrix(this.currentMatrix),
            this.root.mouseX, this.root.mouseY
          )
          startX = this.root.mouseX
          startY = this.root.mouseY
          selectionStart = point
          th.selectRect.x2 = th.selectRect.cx = point[0]
          th.selectRect.y2 = th.selectRect.cy = point[1]
        }, false)
        this.bg.addEventListener('drag', function(ev) {
          var point = CanvasSupport.tMatrixMultiplyPoint(
            CanvasSupport.tInvertMatrix(this.currentMatrix),
            this.root.mouseX, this.root.mouseY
          )
          if (selectionStart && !th.selectRect.visible) {
            var dx = startX - this.root.mouseX
            var dy = startY - this.root.mouseY
            var sqd = dx * dx + dy * dy
            th.selectRect.visible = sqd > 81
          }
          if (th.selectRect.visible) {
            th.selectRect.x2 = point[0]
            th.selectRect.y2 = point[1]
          }
        }, false)
        this.mouseupHandler = function(ev) {
          var point = CanvasSupport.tMatrixMultiplyPoint(
            CanvasSupport.tInvertMatrix(th.currentMatrix),
            th.root.mouseX, th.root.mouseY
          )
          if (selectionStart && th.selectRect.visible) {
            th.selectRect.visible = false
            selectionStart = null
            var selection = objectsInside(th.selectRect)
            if (ev.shiftKey) {
              selection.forEach(context.select.bind(this.context))
            } else if (ev.altKey) {
              selection.forEach(context.deselect.bind(this.context))
            } else {
              context.clearSelection()
              selection.forEach(context.select.bind(this.context))
            }
          } else if (selectionStart && (ev.canvasTarget == th.selectRect || ev.canvasTarget == th.bg)) {
              context.setLobeTarget(point)
            th.selectRect.visible = false
            selectionStart = null
          }
        }
        this.addEventListener('rootChanged', function(ev) {
          if (ev.canvasTarget == this) {
            if (this.root)
              this.root.removeEventListener('mouseup', this.mouseupHandler, false)
            ev.relatedTarget.addEventListener('mouseup', this.mouseupHandler, false)
          }
        }, false)
        this.bg.zIndex = -100
        this.messageLayer = new CanvasNode({
          zIndex : 1000,
          scale : 1 / this.scale
        })
        this.append(this.bg, this.messageLayer)
        this.addFrameListener(function() {
          if (false)
            this.cursor = MOVE_TO_CURSOR
          else
            this.cursor = DEFAULT_CURSOR
        })
        this.showDescription()
      },
      makeNewLobe : function () {
         this.messageLayer.append(new Lobe());
      },
      showDescription : function() {
        var desc = E('div')
        desc.appendChild(E('h1', this.name))
        desc.appendChild(E('div', this.description))
        this.query(desc,
          'Begin editing', function(){
            this.root.dispatchEvent({type: 'started', canvasTarget : this })
          },
          'Back to main menu', function() { this.parentNode.returnToMenu() }
        )
      },

      query : function(header) {
        var div = E('div', {className : 'message'})
        var msg = new ElementNode(div,
          { x : windowWidth/4, y : windowHeight/8, align : 'center' })
        var msgDiv = E('div', header)
        div.appendChild(msgDiv)
        var options = E('div')
        var th = this
        for (var i=1; i<arguments.length; i+=2) {
          var name = arguments[i]
          var callback = arguments[i+1]
          options.appendChild(E('h2', name, {
            onclick : (function(callback){ return function() {
              if (!this.clicked) {
                callback.call(th)
                this.clicked = true
                msg.after(500, msg.removeSelf)
                msg.animateTo('opacity', 0, 500, 'sine')
              }
            }})(callback),
            style: { cursor : 'pointer' }
          }))
        }
        div.appendChild(options)
        msg.opacity = 0
        msg.animateTo('opacity', 1, 500, 'sine')
        this.messageLayer.append(msg)
      },

      notify : function(message, after, duration) {
        if (!after) after = 0
        this.after(after, function(){
          var msg = new ElementNode(E('h3', message),
            { x : windowWidth/4, y : windowHeight/20, align : 'center' })
          if (!duration) duration = 3500 + msg.element.textContent.length * 10
          msg.opacity = 0
          msg.animateTo('opacity', 1, 500, 'sine')
          msg.after(duration, function() {
            this.animateTo('opacity', 0, 500, 'sine')
          })
          msg.after(duration+500, msg.removeSelf)
          this.messageLayer.append(msg)
        })
      },


      returnToMenu : function() {
        if (this.completed) return
        this.failed = true
        this.after(1000, function() {
          this.query(E('h1', "Do you wish to return to menu?"),
            "Return to editing", function() { this.parentNode.tryAgain() },
            "Back to main menu", function() { this.parentNode.returnToMenu() }
          )
        })
      },



    })




    NewBrain = Klass(Editor, {
      width : windowWidth,
      height : windowHeight,
      scale : 1,

      name : "Start new brain",
      description : "Make basic lobes that control reactions to food when hungry.",

      initialize : function() {
        Editor.initialize.call(this)
        this.when('started', function() {

        })
      }

    })


    Menu = Klass(Editor, {
      width : windowWidth,
      height : windowHeight,
      scale : 1,

      initialize : function() {
        Editor.initialize.call(this)
        this.menu = new CanvasNode()
        this.menu.scale = 1
        this.menu.zIndex = 100
        this.append(this.menu)
        this.setupMenu()
        this.selectRect.opacity = 0
      },

      showDescription : function() {},
      makeNewLobe : function () {
                //noop
      },
      setupMenu : function() {
        var elem = E('h1')
        elem.appendChild(T('ELYSIA GENOME EDITOR'))

        var title = new ElementNode(elem, {
          x: windowWidth/4, y: windowHeight/16, zIndex: 1002, align: 'center', cursor: 'default'
        })
        var th = this
        var controls = new CanvasNode()
        var bg = new ElementNode(E('div', {
          style: {
            width : windowWidth+'px',
            height : windowHeight+'px',
            backgroundColor: this.bgColor,
            opacity: 0.5
          }
        }), {zIndex : 1001})
        controls.append(bg)
        controls.display = 'none'
        controls.opacity = 0
        var editorList = E('ol')
        GenomeEditor.editors.slice(1).forEach(function(lvl, i) {
          var li = E('li', E('h3', (i+1) + '. ' + lvl.prototype.name))
          li.onclick = function(){
            if (th.clicked) return
            th.clicked = true
            th.menu.controls.animateTo('opacity', 0, 300, 'sine')
            th.after(300, function() {
              this.parentNode.newEditorState(GenomeEditor.editors.indexOf(lvl))
            })
          }
          li.style.cursor = 'pointer'
          editorList.appendChild(li)
        })
        var editorHeader = E('h2', 'Actions')
        var jump = new ElementNode(editorHeader, {
          zIndex : 1002,
          x : windowWidth/4, y : windowHeight/8,
          align : 'center'
        })
        var editors = new ElementNode(editorList, {
          zIndex : 1002,
          x : windowWidth/4, y : windowHeight/4,
          align : 'center'
        })
        var divider = new Rectangle(windowWidth*.5, 1, {
          centered: true, x: windowWidth/2, y: 128, fill: 'red'
        })
        controls.append(jump, editors, divider)
        this.menu.title = title
        this.menu.controls = controls
        this.menu.append(title)
        this.menu.append(controls)
        this.bg.addEventListener('click', function() {
          if (!th.menuVisible) {
            th.showMenu()
          }
        }, false)
      },

      showMenu : function() {
        if (this.menuVisible) return
        this.menuVisible = true
        var th = this
        this.menu.controls.display = 'block'
        this.menu.controls.animateTo('opacity', 1, 500, 'sine')
        this.menu.after(10000, function() {
          this.controls.animateTo('opacity', 0, 500, 'sine')
          this.after(500, function() {
            this.controls.display = 'none'
            th.menuVisible = false
          })
        })
      }
    })



    GenomeEditor = Klass(CanvasNode, {
      editorIndex : 0,
      editors : [Menu, NewBrain],

      initialize : function(canvasElem) {
        CanvasNode.initialize.call(this)
        this.canvas = new Canvas(canvasElem)
        this.canvas.frameDuration = 30
        this.canvas.append(this)
        this.canvas.fixedTimestep = true
        this.canvas.clear = false
        this.returnToMenu()
        this.setupEtc()
      },

      returnToMenu : function() {
        this.editorIndex = 0
        this.changeEditor(this.editors[this.editorIndex])
      },

      nextEditor : function() {
        this.editorIndex++
        var editor = this.editors[this.editorIndex % this.editors.length]
        this.changeEditor(editor)
      },

      newEditorState : function(idx) {
        this.editorIndex = idx
        var editor = this.editors[this.editorIndex % this.editors.length]
        this.changeEditor(editor)
      },

      tryAgain : function() {
        this.changeEditor(this.editors[this.editorIndex])
      },
      currentEditor : function () {
         return this.editor;
      },
      changeEditor : function(editor) {
        if (this.editor) this.editor.removeSelf()
        if (editor) {
          this.editor = new editor()
          this.append(this.editor)
        }
      },

      Radiation : true,
      setRadiation : function(fb) {
        this.Radiation = fb
      },

      Age : 0.5,
      setAge : function(s) {
        this.Age = s;
      },
      makeNewLobe:function(s) {
         this.currentEditor().makeNewLobe()
      },

      setupEtc : function() {
        this.canvas.updateFps = true
        var debug = E('div')
        var t0 = -1
        var frames = []
        var fc = E.canvas(200, 10)
        var fpsE = T('')
        var elapsedE = T('')
        var realFpsE = T('')
        var elapsedRealE = T('')
        debug.append(fpsE, ' fps (', elapsedE, ' ms to draw scene)', E('br'),
          realFpsE, ' real fps (', elapsedRealE, ' ms between frames)',
          E('br'), fc)
        var fctx = fc.getContext('2d')
        fctx.globalCompositeOperation = 'copy'
        fctx.fillStyle = '#828292'
        this.canvas.addFrameListener(function(t) {
          if (this.updateFps) {
            fctx.drawImage(fc, -1, 0)
            fctx.clearRect(199, 0, 1, 10)
            fctx.fillRect(199, 0, 1, Math.min(100, this.currentRealFps) / 3.3)
            if (Math.floor(t / 500) != t0) {
              t0 = Math.floor(t / 500)
              var fps = (Math.floor(this.fps * 10)/10)
              var elapsed = Math.floor(1000 / this.fps)
              var realFps = (Math.floor(this.realFps * 10)/10)
              var elapsedReal = Math.floor(1000 / this.realFps)
              fpsE.textContent = fps
              elapsedE.textContent = elapsed
              realFpsE.textContent = realFps
              elapsedRealE.textContent = elapsedReal
            }
          }
        })
        this.canvasControlPanel = new GuiConfig({
          object : this.canvas,
          container : $('debug'),
          title : 'Debug',
          controls : [
            'updateFps',
            'playOnlyWhenFocused',
            'drawBoundingBoxes',
            ['useMockContext', 'boolean', {title: "Turn off drawing. Useful for benchmarking the AI."}]
          ]
        })
        this.canvasControlPanel.show()
        this.controlPanel = new GuiConfig({
          object : this,
          container : $('debug'),
          title : 'Lifespan',
          controls : [
            ['Age', '0.0..1.0'],
            'Radiation',
            ['makeNewLobe','function'],
          ]
        })
        this.controlPanel.show()
      }

    })



    init = function() {
      var c = E.canvas(windowWidth, windowHeight)
      var d = E('div', { id: 'screen' })
      d.appendChild(c)
      document.body.appendChild(d)
      GE = new GenomeEditor(c)
    }