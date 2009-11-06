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

    Player = function(t, td) {
      this.target = Player.targets[this.id]
      var th = this
      if (Player.useTargettingAI && (!Player.targets[this.id] || this.target.health <= 0) && (!this.target || this.target.health <= 0 || (Math.random() < 0.3 && this.distanceTo(this.target) > this.weapon.range))) {
        this.target = this.enemies.sort(function(a,b){
          return Math.abs(th.weapon.optimalRange - th.distanceTo(a)) -
                 Math.abs(th.weapon.optimalRange - th.distanceTo(b))
        })[0]
      }
      if (!this.movementMode)
        this.movementMode = 'normal' // (this.weapon instanceof Railgun ? 'defensive' : 'normal')
      if (Player.useMovementAI && this.movementMode != 'manual') {
        if (!this.waypoint && this.target && this.target.health > 0) {
          var angle = this.angleTo(this.target)
          var distance = this.distanceTo(this.target)
          var inRange = (distance < this.weapon.range)
          var optimumRange = (distance < this.weapon.optimalRange)
          var targetInRange = (this.target.weapon && distance < this.target.weapon.range)
          // don't run towards CPU targets
          if (this.movementMode != 'defensive' && (this.movementMode == 'aggressive' || Player.targets[this.id] == this.target)) {
            this.turnToward( angle )
            if (inRange) this.turnToward( angle + (Math.PI / 6) )
            if (optimumRange) this.turnToward( angle + Math.PI )
            this.moveAt(1)
          }
          // run away from CPU targets that we can snipe
          if (inRange && !targetInRange) {
            this.turnToward( angle + Math.PI )
            this.moveAt(1)
          }
        }
      }
    }
    Player.useMovementAI = true
    Player.useTargettingAI = true
    Player.targets = {}
    Player.selection = []
    Player.toggleSelect = function(s) {
      if (this.selection.indexOf(s.id) == -1)
        this.select(s)
      else
        this.deselect(s)
    }
    Player.clearSelection = function() {
      var dict = this.selection.dict
      while (this.selection.length > 0) {
        this.deselect(this.selection.dict[this.selection[0]])
      }
    }
    Player.select = function(s) {
      if (!this.selection.dict) this.selection.dict = {}
      if (!this.selection.formation) this.selection.formation = new Formation()
      s.root.dispatchEvent({type : 'select', canvasTarget: s})
      this.selection.dict[s.id] = s
      this.selection.formation.addShip(s)
      this.selection.push(s.id)
    }
    Player.deselect = function(s) {
      if (!this.selection.dict) this.selection.dict = {}
      if (!this.selection.formation) this.selection.formation = new Formation()
      s.root.dispatchEvent({type : 'deselect', canvasTarget: s})
      delete this.selection.dict[s.id]
      this.selection.formation.removeShip(s)
      this.selection.deleteFirst(s.id)
    }
    Player.setWaypoint = function(wp) {
      if (!this.selection.formation) this.selection.formation = new Formation()
      var sc = this.getSelectionCenter()
      var rotation = Curves.lineAngle(sc, wp)
      this.selection.formation.setWaypoint(wp, rotation)
    }
    Player.getSelectionCenter = function() {
      var x = 0
      var y = 0
      for(var i=0; i<this.selection.length; i++) {
        var s = this.selection.dict[this.selection[i]]
        x += s.x
        y += s.y
      }
      return [ x / this.selection.length, y / this.selection.length ]
    }
    Player.setTarget = function(t) {
      this.selection.forEach(function(s) {
        if (Player.useMovementAI)
          delete Player.selection.dict[s].waypoint
        Player.targets[s] = t
      })
    }

    Formation = Klass({
      initialize : function() {
        this.ships = []
        for (var i=0; i<arguments.length; i++)
          this.ships.push(arguments[i])
      },

      addShip : function(ship) {
        if (this.ships.indexOf(ship) == -1)
          this.ships.push(ship)
      },

      removeShip : function(ship) {
        this.ships.deleteFirst(ship)
      },

      formationFunction : function(ships) {
        return ships.map(function(s, i, th) {
          return [-70 * (i / 3), 70 * (i % 3)]
        })
      },

      setWaypoint : function(point, rotation) {
        var waypoints = this.formationFunction(this.ships)
        for (var i=0; i<waypoints.length; i++) {
          var wp = waypoints[i]
          var ship = this.ships[i]
          var p = V.add(point, V.rotate(wp, rotation))
          ship.waypoint = {x:p[0], y:p[1], rotation:rotation}
        }
      }
    })
  



    ControlledNode = Klass(CanvasNode, {
      rotation : 0,
      targetAngle : 0,
      sinceLastTick : 0.0,

      catchMouse : false,
      turningSpeed : 1,
      movementSpeed : 20,
      moveSpeedFactor : 0,
      frame : 0,
      id : 0,

      initialize : function() {
        this.id = ControlledNode.id++
        CanvasNode.initialize.apply(this, arguments)
        this.addFrameListener(this.updatePosition)
      },


      updatePosition : function(t, dt) {
        var d = Curves.angularDistance(this.rotation, this.targetAngle)
        if (d > 0)
          d = Math.min(d, this.turningSpeed * (dt/1000))
        else
          d = Math.max(d, -this.turningSpeed * (dt/1000))
        this.rotation += d
        var xf = Math.cos(this.rotation)
        var yf = Math.sin(this.rotation)
        this.x += xf * this.movementSpeed * this.moveSpeedFactor * (dt/1000)
        this.y += yf * this.movementSpeed * this.moveSpeedFactor * (dt/1000)
      }
    })



    Ship = Klass(ControlledNode, {
      isShip : true,
      target : null,
      health : 100,
      catchMouse : true,

      turningSpeed : 1,
      movementSpeed : 40,
      z: 0,

      initialize : function(color, weapon, pointDefense, x, y, noWarp, health) {
        ControlledNode.initialize.call(this)
        this.zIndex = 0 + 0.001 * (Ship.z++ % 1000)
        this.stroke = color
        this.team = color
        this.weapon = weapon
        this.weapon.ship = this
        this.pointDefense = pointDefense
        this.pointDefense.ship = this
        this.x = x
        this.y = y
        if (health)
          this.health = health
        this.rotation = Math.random() * Math.PI*2
        this.model = new Polygon([20,0, -10,15, -10,-15])
        this.model.strokeWidth = 2
        //this.core = new Circle(5)
        this.hardpoint = this.model.clone()
        Object.extend(this.hardpoint, {
          scale: 0.2 + this.weapon.techLevel * 0.1,
          centered:true,
          stroke: false,
          fill: this.weapon.color,
          weapon: this.weapon
        })
        this.hardpoint.addFrameListener(function() {
          this.fill = this.weapon.color
          this.scale = 0.2 + this.weapon.techLevel * 0.1
        }, false)
        this.append(this.weapon)
        this.append(this.pointDefense)
        this.model.append(this.hardpoint)
        this.append(this.model)
        this.healthBar = new Rectangle(20, 2, {
          fill: color, stroke: false,
          centered: true, cy: 30
        })
        this.selected = new Circle(35, {opacity: 0})
        this.targetMarker = new Rectangle(40, 40, {
          rotation: Math.PI/4,
          centered: true, stroke: this.stroke, visible: false, opacity: 0.5,
          catchMouse : false
        })
        this.targetLine = new Line(0,0,0,0, {
          stroke: this.stroke, visible: false, opacity : 0.5,
          catchMouse : false
        })
        this.waypointLine = new Line(0,0,0,0, {
          stroke: '#448866', visible: false, opacity : 0.5,
          catchMouse : false
        })
        this.append(this.selected)
        if (!this.maxHealth) this.maxHealth = this.health
        this.addEventListener('mousedown', function(ev) {
          if (this.strategicAI == Player) {
            if (!ev.shiftKey)
              Player.clearSelection()
            Player.toggleSelect(this)
          } else {
            Player.setTarget(this)
          }
        }, false)
        this.addEventListener('select', function(ev) {
          this.selected.opacity = 0.5
          this.waypointLine.opacity = 0.5
          this.targetLine.opacity = 0.5
        }, false)
        this.addEventListener('deselect', function(ev) {
          this.selected.opacity = 0
          this.waypointLine.opacity = 0.15
          this.targetLine.opacity = 0.1
        }, false)
        if (!noWarp) this.warpIn()
      },


      firedShots : 0,

    })


    Editor = Klass(CanvasNode, {
      bgColor : 'rgb(0,0,0)',
      bgOpacity : 0.15,

      playerTeam : '#aa2222',
      enemyTeam : '#2266aa',

      initialize : function() {
        CanvasNode.initialize.call(this)
        this.ships = {}
        this.bg = new Rectangle(this.width, this.height)
        this.bg.fill = this.bgColor
        this.bg.fillOpacity = this.bgOpacity
        var selectionStart, startX, startY
        var th = this
        var playerShipsInside = function(rect) {
          return th.childNodes.filter(function(s) {
            var x1 = Math.min(rect.cx, rect.x2)
            var x2 = Math.max(rect.cx, rect.x2)
            var y1 = Math.min(rect.cy, rect.y2)
            var y2 = Math.max(rect.cy, rect.y2)
            return s.isShip && s.strategicAI == Player &&
                   (s.x >= x1 && s.x <= x2 && s.y >= y1 && s.y <= y2)
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
            var selection = playerShipsInside(th.selectRect)
            if (ev.shiftKey) {
              selection.forEach(Player.select.bind(Player))
            } else if (ev.altKey) {
              selection.forEach(Player.deselect.bind(Player))
            } else {
              Player.clearSelection()
              selection.forEach(Player.select.bind(Player))
            }
          } else if (selectionStart && (ev.canvasTarget == th.selectRect || ev.canvasTarget == th.bg)) {
            Player.setWaypoint(point)
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
        this.destroyedHandler = this.destroyed.bind(this)
        this.when('teamDestroyed', function(ev) {
          if (ev.detail == this.enemyTeam) this.enemyTeamDestroyed(ev.detail)
          else if (ev.detail == this.playerTeam) this.gameOver()
        })
        this.showDescription()
      },

      enemyTeamDestroyed : function(team) {
        this.levelCompleted()
      },

      showDescription : function() {
        var desc = E('div')
        desc.appendChild(E('h1', this.name))
        desc.appendChild(E('div', this.description))
        this.query(desc,
          'Begin editing', function(){
            this.root.dispatchEvent({type: 'started', canvasTarget : this })
          },
          'Back to main menu', function() { this.parentNode.gameOver() }
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
            { x : windowWidth/2, y : windowHeight/20, align : 'center' })
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


      gameOver : function() {
        if (this.completed) return
        this.failed = true
        this.after(1000, function() {
          this.query(E('h1', "Your fleet was destroyed"),
            "Try again", function() { this.parentNode.tryAgain() },
            "Back to main menu", function() { this.parentNode.gameOver() }
          )
        })
      },

      levelCompleted : function() {
        if (this.failed) return
        this.after(1000, function() {
          if (this.failed) return
          this.completed = true
          this.query(E('h1', "Editor complete"),
            "Next level", function() { this.parentNode.nextEditor() },
            "Play again", function() { this.parentNode.tryAgain() },
            "Back to main menu", function() { this.parentNode.gameOver() }
          )
        })
      },


      destroyed : function(ev) {
        this.ships[ev.canvasTarget.team]--
        if (this.ships[ev.canvasTarget.team] < 1) {
          this.root.dispatchEvent({
            type : 'teamDestroyed',
            detail : ev.canvasTarget.team,
            canvasTarget : this
          })
        }
      }

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
      playerTeam : null,
      enemyTeam : null,

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

      setupMenu : function() {
        var elem = E('h1')
        elem.appendChild(T('ELYSIA GENOME EDITOR'))
/*        elem.appendChild(E('span', '+', {style: {color: 'red'}}))
        elem.appendChild(T('FLEET'))*/
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
        var levelList = E('ol')
        GenomeEditor.levels.slice(1).forEach(function(lvl, i) {
          var li = E('li', E('h3', (i+1) + '. ' + lvl.prototype.name))
          li.onclick = function(){
            if (th.clicked) return
            th.clicked = true
            th.menu.controls.animateTo('opacity', 0, 300, 'sine')
            th.after(300, function() {
              this.parentNode.jumpToLevel(GenomeEditor.levels.indexOf(lvl))
            })
          }
          li.style.cursor = 'pointer'
          levelList.appendChild(li)
        })
        var levelHeader = E('h2', 'Actions')
        var jump = new ElementNode(levelHeader, {
          zIndex : 1002,
          x : windowWidth/4, y : windowHeight/8,
          align : 'center'
        })
        var levels = new ElementNode(levelList, {
          zIndex : 1002,
          x : windowWidth/4, y : windowHeight/4,
          align : 'center'
        })
        var divider = new Rectangle(windowWidth*7/8, 1, {
          centered: true, x: windowWidth/2, y: 87.5, fill: 'red'
        })
        controls.append(jump, levels, divider)
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
      levelIndex : 0,
      levels : [Menu, NewBrain],

      initialize : function(canvasElem) {
        CanvasNode.initialize.call(this)
        this.canvas = new Canvas(canvasElem)
        this.canvas.frameDuration = 30
        this.canvas.append(this)
        this.canvas.fixedTimestep = true
        this.canvas.clear = false
        this.gameOver()
        this.setupEtc()
      },

      gameOver : function() {
        this.levelIndex = 0
        this.changeLevel(this.levels[this.levelIndex])
      },

      nextLevel : function() {
        this.levelIndex++
        var level = this.levels[this.levelIndex % this.levels.length]
        this.changeLevel(level)
      },

      jumpToLevel : function(idx) {
        this.levelIndex = idx
        var level = this.levels[this.levelIndex % this.levels.length]
        this.changeLevel(level)
      },

      tryAgain : function() {
        this.changeLevel(this.levels[this.levelIndex])
      },

      changeLevel : function(level) {
        if (this.level) this.level.removeSelf()
        if (level) {
          this.level = new level()
          this.append(this.level)
        }
      },

      fastExplosions : false,
      setFastExplosions : function(fe) {
        this.fastExplosions = fe
        Explosion.fastExplosions = fe
      },

      noExplosions : false,
      setNoExplosions : function(fe) {
        this.noExplosions = fe
        Explosion.prototype.visible = !fe
      },

      Radiation : true,
      setRadiation : function(fb) {
        this.Radiation = fb
      },

      Age : 0.5,
      setAge : function(s) {
        this.Age = s;
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
            'Radiation'
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
