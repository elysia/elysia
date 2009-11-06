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


    Explosion = Klass(CanvasNode, {
      catchMouse : false,
      cursor : 'default',

      circleGradient : new Gradient({
        type : 'radial',
        endRadius : 15,
        colorStops : [
          [ 0.0, "rgba(190,105,90,1)" ],
          [ 0.25, "rgba(5,30,80,0.4)" ],
          [ 1, "rgba(10,0,40,0)" ]
        ]
      }),

      initialize : function(size) {
        CanvasNode.initialize.call(this)
        var main = new Circle(15)
        main.fill = this.circleGradient
        main.compositeOperation = 'lighter'
        this.zIndex = 10
        this.main = main
        this.append(main)
        this.size = size
        this.addFrameListener(this.blowup)
        this.after(500, this.removeSelf)
      },

      blowup : function(t, dt) {
        if (this.startTime == null)
          this.startTime = t
        var elapsed = Math.min(500, t - this.startTime)
        var fac = 0.48 * 0.004 * Math.PI
        this.main.scale = 1 + this.size *
                          (Explosion.fastExplosions ? 1 :
                                                      Math.tan(elapsed * fac))
        if (isNaN(this.main.scale)) this.main.scale = 60000
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
        this.addFrameListener(this.callAI)
        this.addFrameListener(this.updatePosition)
        this.addFrameListener(this.updateHealth)
      },

      callAI : function(t,dt) {
        if (this.root) {
          if (dt && !isNaN(dt)) this.sinceLastTick += dt
          this.tick = this.sinceLastTick >= this.root.frameDuration
          if (this.tick) this.sinceLastTick = 0.0
        }

        if (this.tick) {
          if (this.frame % 10 == 0) {
            // stagger frames
            if (this.frame == 0) this.frame += Math.floor(Math.random()*10)
            this.moveSpeedFactor = 0
            this.targetAngle = this.rotation
            this.ai(t,dt)
          }
          this.hitDetect(t,dt)
          this.frame++
        }
      },

      ai : function(t, dt) {
      },

      hitDetect : function(t,dt) {
      },

      predictAngleToTarget : function(speed) {
        var fx = Math.cos(this.target.rotation)
        var fy = Math.sin(this.target.rotation)
        var s = this.target.movementSpeed * this.target.moveSpeedFactor
        var tv = [fx * s, fy * s]
        var d_px = this.target.x - this.x
        var d_py = this.target.y - this.y
        var a = d_px*d_px + d_py*d_py
        var b = 2*d_px*tv[0] + 2*d_py*tv[1]
        var c = tv[0]*tv[0] + tv[1]*tv[1] - speed*speed
        var d = b*b - 4*a*c
        if (d < 0) {
          return this.angleTo(this.target)
        }
        var t = 2 * a / (-b + Math.sqrt(d))
        var est_p_t = [
          this.target.x + tv[0]*t,
          this.target.y + tv[1]*t
        ]
        return Math.atan2(est_p_t[1]-this.y, est_p_t[0]-this.x)
      },

      turnToward : function(a) {
        this.targetAngle = a
      },

      moveAt : function(speedFactor) {
        this.moveSpeedFactor = Math.min(1, Math.max(-1, speedFactor))
      },

      updateHealth : function(t,dt) {
        if (this.healthBar) {
          this.healthBar.width = Math.max(0, parseInt(this.health / 2.5))
          this.healthBar.opacity = this.opacity
          this.healthBar.x = this.x
          this.healthBar.y = this.y
        }
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

      warpIn : function() {
        this.opacity = 0
        this.aiDisabled = true
        this.warpModel = new Spiral(0, {stroke: 'blue', zIndex : -1})
        this.warpModel.animateTo('endAngle', 40, 500, 'square')
        this.warpModel.after(500, function() {
          this.animateTo('endAngle', 0, 500, 'square')
          this.after(500, this.removeSelf)
        })
        this.animateTo('opacity', 1, 1000, 'sine')
        this.model.animate('rotation', -10, 0, 1000, 'sqrt')
        this.after(1000, function() {
          this.aiDisabled = false
        })
//         this.append(this.warpModel)
      },

      /**
        Tactical AI:
          - takes care of point defense
          - fires guns at the current target
          - steers the vessel towards current waypoint
      */
      tacticalAI : function(t, dt) {
        if (this.aiDisabled) return
        var siblings = this.parentNode.childNodes
        var th = this
        var targets = []
        if (this.pointDefense.readyToFire) {
          var missiles = siblings.filter(function(s) {
            return s.isMissile
          })
          var incoming = missiles.filter(function(s) {
            return s.target.team == th.team
          }).sort(function(a,b){
            return th.pointDefense.optimalRange -
                  Math.abs(th.distanceTo(a) - th.distanceTo(b))
          })
          var threats = incoming.filter(function(s) {
            return s.target == th
          }).sort(function(a,b){
            return Math.abs(th.pointDefense.optimalRange - th.distanceTo(a)) -
                  Math.abs(th.pointDefense.optimalRange - th.distanceTo(b))
          })
          var targets = threats.concat(incoming).concat(this.enemies).filter(function(s) {
            return (!s.target || s.target.health > 0) &&
                  (s.isMissile ? (th.distanceTo(s) < th.pointDefense.range) :
                                  (th.distanceTo(s) < th.pointDefense.optimalRange))
          })
          this.intercept(targets)
        }
        var target = this.target || targets[0]
        if (target && target.health > 0) {
          var distance = this.distanceTo(target)
          if (distance < this.weapon.range && this.weapon.readyToFire) {
            if (Math.random() < 0.7) this.fireAt(target)
          }
        }
        if (this.waypoint) {
          if (this.distanceTo(this.waypoint) < 5) {
            this.moveAt(0)
            if (Math.abs(Curves.angularDistance(this.rotation, this.waypoint.rotation)) < 0.1)
              delete this.waypoint
            else
              this.turnToward(this.waypoint.rotation)
          } else {
            this.turnToward(this.angleTo(this.waypoint))
            this.moveAt(this.distanceTo(this.waypoint) / this.movementSpeed)
          }
        }
      },

      /**
        Strategic AI:
          - sets the main gun target
          - sets waypoints
      */
      strategicAI : function(t, dt) {
        if (this.aiDisabled) return
        var th = this
        if (!this.target || this.target.health <= 0 || (Math.random() < 0.3 && this.distanceTo(this.target) > this.weapon.range)) {
          this.target = this.enemies.sort(function(a,b){
            return Math.abs(th.weapon.optimalRange - th.distanceTo(a)) -
                  Math.abs(th.weapon.optimalRange - th.distanceTo(b))
          })[0]
        }
        if (this.target && this.target.health > 0) {
          var angle = this.angleTo(this.target)
          var distance = this.distanceTo(this.target)
          if (distance < this.weapon.range) {
            this.turnToward( angle + (Math.PI / 6) )
          } else if (distance > (this.weapon.optimalRange)) {
            this.turnToward( angle )
          } else if (Math.random() < 0.8) {
            this.turnToward( angle + (Math.PI / 2) )
          }
          if (distance < this.weapon.optimalRange) {
            this.turnToward( angle + (Math.PI * 0.85) )
          }
          this.moveAt(1)
        }
      },

      hitDetect : function(t, dt) {
        ControlledNode.hitDetect.apply(this, arguments)
        if (this.health <= 0) {
          Player.deselect(this)
          this.selected.opacity = 0
          this.targetLine.removeSelf()
          this.targetMarker.removeSelf()
          this.waypointLine.removeSelf()
          this.healthBar.removeSelf()
        } else {
          // FIXME Make the target marker fly to target with a spin
          if (this.healthBar.parent != this.parent)
            this.parent.append(this.healthBar)
          if (this.strategicAI == Player)
            this.cursor = SELECT_CURSOR
          else if (Player.selection.length > 0)
            this.cursor = TARGET_CURSOR
          else
            this.cursor = DEFAULT_CURSOR
          this.healthBar.cursor = this.cursor
          if (this.target && this.target.health > 0 && (this.strategicAI == Player || this.target.strategicAI == Player)) {
            if (!this.targetMarker.parent)
              this.parent.append(this.targetMarker, this.targetLine)
            this.targetLine.x1 = this.x
            this.targetLine.y1 = this.y
            this.targetLine.x2 = this.targetMarker.x = this.target.x
            this.targetLine.y2 = this.targetMarker.y = this.target.y
            this.targetLine.visible = (Player.targets[this.id] == this.target && !this.waypoint)
            this.targetMarker.visible = true
          } else {
            this.targetLine.visible = this.targetMarker.visible = false
          }
          if (this.waypoint && this.strategicAI == Player) {
            if (!this.waypointLine.parent)
              this.parent.append(this.waypointLine)
            this.waypointLine.x1 = this.x
            this.waypointLine.y1 = this.y
            this.waypointLine.x2 = this.waypoint.x
            this.waypointLine.y2 = this.waypoint.y
            this.waypointLine.visible = true
          } else {
            this.waypointLine.visible = false
          }
        }
      },

      ai : function(t, dt) {
        // don't leave the mission area lest you wish to die
        if ((this.x < -50) || (this.y < -50) ||
            (this.x > this.parent.width+50) || (this.y > this.parent.height+50))
          this.health -= 3

        if (this.health > 0) {
          var siblings = this.parentNode.childNodes
          var th = this
          this.enemies = siblings.filter(function(s) {
            return s.isShip && s.team != th.team
          })
          this.friends = siblings.filter(function(s) {
            return s.isShip && s.team == th.team
          })
          this.strategicAI(t, dt)
          this.tacticalAI(t, dt)
        } else if (!this.blowup) {
          var sz = (this.maxHealth / 20)*(1 + 0.1*this.weapon.techLevel)
          var ex = new Explosion(0.25*sz + Math.random())
          ex.x = this.x
          ex.y = this.y
          this.parent.append(ex)
          var exps = sz * 0.2 + Math.random() * sz
          var ssz = Math.sqrt(sz / 5)
          for (var i=0; i<exps; i++) {
            this.parent.after(200+i*60*Math.random(), (function(i){
              return function() {
                var e = new Explosion(Math.random()*sz/5)
                var dx = Math.random()
                dx *= dx
                dx += i / exps
                dx *= (Math.random() < 0.5 ? -1 : 1)
                e.x = ex.x + dx * 5 * sz
                var dx = Math.random()
                dx *= dx
                dx += i / exps
                dx *= (Math.random() < 0.5 ? -1 : 1)
                e.y = ex.y + dx * 5 * sz
                this.append(e)
              }
            })(i))
          }
          this.root.dispatchEvent({type: 'destroyed', canvasTarget: this})
          this.removeSelf()
          this.blowup = true
        }
      },

      intercept : function(targets) {
        if (targets.length == 0) return
        var i = 0
        var j = 0
        while (this.pointDefense.readyToFire) {
          this.pointDefense.fireAt(targets[i])
          i = (i+1) % targets.length
          if (i == 0) j++
          if (j == 1) return
        }
      },

      firedShots : 0,
      fireAt : function(target) {
        for (var i=0; i < this.weapon.salvos / 4; i++) {
          if (this.weapon.readyToFire) {
            this.weapon.rotation = (Math.PI) * ((this.firedShots % 2) - 0.5)
            this.weapon.fireAt(target)
            this.firedShots++
          }
        }
      }

    })




    Projectile = Klass(ControlledNode, {
      zIndex : 2,
      catchMouse : false,

      initialize : function(target, weapon, x, y, rot) {
        this.targetingFunction = this.angleTo
        Object.extend(this, weapon)
        ControlledNode.initialize.call(this)
        this.elapsed = 0
        this.owner = weapon
        this.target = target
        this.x = x
        this.y = y
        this.rotation = rot
        this.model = new Rectangle(this.movementSpeed / 16, this.height, {centered : true})
        this.append(this.model)
        this.fill = this.color
        if (this.projectileHealth)
          this.health = this.projectileHealth
        this.maxHealth = this.health
        this.initAI()
      },

      selfDestruct : function() {
        if (!this.parent) return
        var ex = new Explosion(this.maxHealth * 0.01)
        ex.x = this.x
        ex.y = this.y
        this.parent.append(ex)
        this.removeSelf()
        return false
      },

      ai : function(t,dt) {
        if (!this.target) return
        var angle = this.targetingFunction(this.target)
        this.turnToward( angle )
        this.moveAt(1)
      },

      hitDetect : function(t, dt) {
        var targetAlive = this.target.health > 0
        this.elapsed += dt
        if (!this.target) return
        var distance = this.distanceTo(this.target)
        if (distance < this.hitRadius) this.hit()
        if (targetAlive && this.target.health <= 0)
          this.owner.gainExp(this.target)
        if (this.health <= 0) return this.selfDestruct()
        return true
      },

      hit : function() {
        this.target.health -= this.damage
        this.health -= this.hitDamageToSelf
        var ex = new Explosion(this.damage * 0.01)
        ex.x = this.x
        ex.y = this.y
        this.parent.append(ex)
      }

    })

    Weapon = Klass(CanvasNode, {
      catchMouse : false,
      movementSpeed : 100,
      turningSpeed : 1,
      health : 20,
      projectileHealth : 1,
      hitDamageToSelf : 1,
      range : 200,
      optimalRange : 100,
      reloadTime : 1000,
      salvos : 5,
      techLevel : 0,
      rotation : 0,
      height: 2,
      color: 'white',
      x : 0, y : 0, rotation : 0,
      projectile : Projectile,
      readyToFire : true,

      initialize : function(techLevel) {
        CanvasNode.initialize.call(this)
        this.freeSalvos = this.salvos
        if (techLevel)
          this.techLevel = techLevel
      },

      gainExp : function(target) {
        var targetTech = target.techLevel
        if (target.weapon)
          targetTech = target.weapon.techLevel
        else if (target.damage)
          targetTech *= 0.02 * Math.max(1, target.damage)
//         this.techLevel += 0.5 * ((targetTech+1) / (this.techLevel+1))
      },

      fireAt : function(target) {
        if (this.freeSalvos < 1) return false
        this.freeSalvos--
        this.rx = this.x + this.ship.x
        this.ry = this.y + this.ship.y
        this.rrot = this.rotation + this.ship.rotation
        var proj = this.createProjectile(target)
        this.ship.parent.append(proj)
        this.after(this.reloadTime, this.reload)
        this.readyToFire = (this.freeSalvos > 0)
      },

      reload : function() {
        this.freeSalvos++
        this.readyToFire = (this.freeSalvos > 0)
      },

      createProjectile : function(target) {
        return new this.projectile(target, this, this.rx, this.ry, this.rrot)
      }
    })



    RapidFireRailgun = Klass(Weapon, {
      isMissile : true,
      movementSpeed : 500,
      turningSpeed : 0,
      projectileHealth : 1,
      hitRadius : 10,
      damage : 100,
      range : 500,
      optimalRange : 250,
      reloadTime : 200,
      salvos: 1,
      color: '#5533ff',
      height: 2,
      damageType : 'kinetic',

      initAI : function() {
        this.rotation = this.predictAngleToTarget(this.movementSpeed)
        this.reloadTime *= Math.pow(0.8, this.techLevel)
        this.movementSpeed *= Math.pow(1.1, this.techLevel)
        this.hitRadius *= Math.pow(1.1, this.techLevel)
        this.range *= Math.pow(1.1, this.techLevel)
        this.after(1000, this.removeSelf)
        var smoke = new CanvasNode({
          stroke: '#ba88ba',
          x: this.x,
          y: this.y,
          rotation: this.rotation
        })
        var x = 40+Math.random()*10
        smoke.append(new Circle(5, {x:x, scale: [0.2, 1]}))
        smoke.animate('opacity', 0.7, 0, 300, 'sqrt')
        smoke.childNodes[0].animateTo('x', x-this.movementSpeed*0.01, 300, 'sqrt')
        smoke.childNodes[0].animateToFactor('scale', 1+this.movementSpeed*(0.004+Math.random()*0.003), 300, 'sqrt')
        smoke.after(300, smoke.removeSelf)
        this.afterFrame(1, function() {
          if (this.parentNode)
            this.parentNode.append(smoke)
        })
      },

      ai : function(t, dt) {
        this.moveAt(1)
      },
      hit : function() {
        Projectile.hit.apply(this, arguments)
        var rs = Math.max(0, this.movementSpeed*this.moveSpeedFactor*(0.5+0.5*Math.random()))
        var streak = new CanvasNode({
          x:this.x, y:this.y, rotation: this.rotation
        })
        streak.after(300, streak.removeSelf)
        this.parent.append(streak)
        var c = new Circle(6, {x: 30, scale:[0.2,1], strokeWidth:5, fill:'none', stroke:'#ba88ba'})
        if (rs <= 1) rs = 1
        c.animateTo('x', 30+rs*0.04, rs*0.4, 'sqrt')
        c.animateToFactor('radius', 2 + rs*0.005*Math.random()*2, rs*0.3, 'sqrt')
        c.animate('opacity', 1, 0, rs*0.4, 'sqrt')
        streak.append(c)
      }
    })

    Missiles = Klass(Weapon, {
      isMissile : true,
      movementSpeed : 160,
      turningSpeed : 3,
      projectileHealth : 1,
      hitRadius : 10,
      damage : 20,
      range : 500,
      optimalRange : 250,
      reloadTime : 3000,
      salvos: 8,
      color: '#22ccff',
      height: 1.5,
      damageType : 'explosive',

      initAI : function() {
        this.model.width *= 2
        this.reloadTime *= Math.pow(0.8, this.techLevel)
        this.turningSpeed *= Math.pow(1.1, this.techLevel)
        this.movementSpeed *= Math.pow(1.1, this.techLevel)
        this.damage *= Math.pow(1.25, this.techLevel)
        if (this.techLevel >= 4) {
          this.salvos += 2
          this.targetingFunction = this.predictAngleTo
        }
        this.after(5000, this.removeSelf)
      },

      predictAngleTo : function(target) {
        return this.predictAngleToTarget(this.movementSpeed)
      },

      hitDetect : function(t,dt) {
        if (!this.target) return
        if (!Projectile.hitDetect.apply(this, arguments))
          return false
        this.movementSpeed += dt * 0.06
        var distance = this.distanceTo(this.target)
        if (distance < 75) { // last sprint with low-latency tracking
          if (this.techLevel >= 3) {
            var angle = this.targetingFunction(this.target)
            this.turnToward( angle )
          }
        } else if (distance < 150) { // try to dodge point defense
          if (this.techLevel >= 3) {
            this.targetAngle += (Math.random() - 0.5) * 0.4 * Math.max(1, distance)*0.01
          }
        }
      },

      hit : function() {
        Projectile.hit.apply(this, arguments)
        if (!Explosion.fastExplosions) {
          var streak = new Circle(Math.random()*4+this.damage*0.18,{
            rotation:this.rotation,
            x:this.x, y:this.y,
            strokeWidth: 1.5,
            scale: [0.5+Math.random()*0.5, 1],
            stroke: '#da88fa'
          })
          streak.animate('opacity', 0.8, 0, 400, 'sqrt')
          streak.animateToFactor('scale', 5, 400, 'sqrt')
          streak.after(400, streak.removeSelf)
          this.parent.append(streak)
        }
      }
    })

    Beam = Klass(Weapon, {
      movementSpeed : 200,
      turningSpeed : 6,
      health : 1,
      range : 200,
      optimalRange : 50,
      reloadTime : 500,
      hitRadius : 6,
      damage : 15,
      salvos: 5,
      color: '#cc44ff',
      sdTime : 1250,
      height : 1,
      isMissile : true,
      damageType : 'electric',

      initAI : function() {
        this.after(this.sdTime, this.removeSelf)
        this.range *= Math.pow(1.1, this.techLevel)
        this.movementSpeed *= Math.pow(1.1, this.techLevel)
        this.reloadTime *= Math.pow(0.9, this.techLevel)
        this.damage *= Math.pow(1.1, this.techLevel)
        if (this.techLevel >= 1) {
          this.turningSpeed *= 1.5
        }
        if (this.techLevel >= 2) {
          this.hitRadius *= 1.5
        }
        if (this.techLevel >= 3) {
          this.range *= 1.25
          this.rotation = this.predictAngleToTarget(this.movementSpeed)
        } else {
          this.rotation = this.angleTo(this.target)
        }
        if (this.techLevel >= 4) {
          this.damage *= 1.5
          this.turningSpeed *= 1.5
        }
        this.rotation += 0.5*(Math.random() - 0.5)
      },

      ai : function(t, dt) {
        var angle = this.angleTo(this.target)
        var distance = this.distanceTo(this.target)
        this.turnToward( angle )
        this.moveAt(1)
        if (distance < 100 && this.target.health > 0 && Math.random() < 0.5) {
          var nst = this.sdTime - this.elapsed - 250
          if (nst > 0) {
            var subBeam = new Projectile(this.target, this, this.x, this.y, this.rotation)
            subBeam.sdTime = nst
            subBeam.after(subBeam.sdTime, subBeam.removeSelf)
            subBeam.rotation += Math.random()*2 - 1
            this.parentNode.append(subBeam)
          }
        }
      },

      hitDetect : function(t,dt) {
        if (Beam.fastBeams && this.target.health <= 0) return this.removeSelf()
        Projectile.hitDetect.apply(this, arguments)
      },

      hit : function() {
        var bonus = 10 * (1-(this.elapsed / (this.sdTime+1)))
        this.target.health -= this.damage + bonus
        this.health = 0
        if (!Beam.fastBeams) {
          var dp = Math.max(0, this.damage+bonus)
          if (dp > this.target.maxHealth) dp = this.target.maxHealth
          var streak = new Circle(Math.random()*5+(dp)*0.1,{
            rotation:this.rotation,
            x:this.x, y:this.y,
            strokeWidth: 1,
            scale: 1,
            compositeOperation: 'lighter',
            stroke: '#a500a5'
          })
          streak.animate('opacity', 0.8, 0, 300, 'sqrt')
          streak.animateToFactor('scale', 5, 300, 'sqrt')
          streak.after(300, streak.removeSelf)
          this.parent.append(streak)
        }
      }

    })

    Railgun = Klass(Weapon, {
      isMissile : true,
      movementSpeed : 550,
      turningSpeed : 0.2,
      projectileHealth : 5,
      range : 600,
      optimalRange : 400,
      reloadTime : 4000,
      salvos: 1,
      color: 'orange',
      hitRadius : 10,
      damageType : 'kinetic',

      initAI : function() {
        var dx = Math.cos(this.ship.rotation)
        var dy = Math.sin(this.ship.rotation)
        this.x += dx * 4
        this.y += dy * 4
        this.rotation = this.predictAngleToTarget(this.movementSpeed) +
                        (Math.random() - 0.5) * 0.2 * (1/(1+this.techLevel))
        if (this.techLevel >= 1) {
          this.movementSpeed += 50
          this.reloadTime *= 0.75
        }
        if (this.techLevel >= 2) {
          this.turningSpeed *= 1.5
        }
        if (this.techLevel >= 3) {
          this.movementSpeed *= 1.2
          this.hitRadius *= 1.2
          this.range *= 1.2
        }
        if (this.techLevel >= 4) {
          this.movementSpeed *= 1.3
          this.hitRadius *= 1.3
          this.range *= 1.4
          this.reloadTime *= 0.66
        }
        var compressionWave = new CanvasNode({
          scale: [0.2, 0.8],
          stroke: '#ba88ba',
          strokeWidth: 2,
          x: this.x,
          y: this.y,
          rotation: this.rotation
        })
        compressionWave.append(new Circle(30, {x: 60}))
        var dur = 400
        compressionWave.animateToFactor('scale', 1.4 + this.movementSpeed*0.001, dur, 'sqrt')
        compressionWave.animate('opacity', 1, 0, dur, 'sqrt')
        compressionWave.after(dur, compressionWave.removeSelf)
        var smoke = new CanvasNode({
          fill: '#ba88ba',
          x: this.x,
          y: this.y,
          rotation: this.rotation
        })
        smoke.append(new Rectangle(this.movementSpeed / 40, 2, {centered:true}))
        smoke.animate('opacity', 1, 0, 600, 'sqrt')
        smoke.childNodes[0].animateTo('x', -this.movementSpeed*0.1, 600, 'sqrt')
        smoke.after(600, smoke.removeSelf)
        this.afterFrame(1, function() {
          this.parentNode.append(compressionWave)
          this.parentNode.append(smoke)
        })
        this.after(3000, this.removeSelf)
      },

      ai : function(t, dt) {
        this.moveAt(1-Math.sqrt(this.elapsed/6000))
        var angle = this.predictAngleToTarget(this.movementSpeed * this.moveSpeedFactor)
        this.turnToward( angle )
      },

      hit : function() {
        var rs = Math.max(0, this.movementSpeed*this.moveSpeedFactor)
        var streak = new CanvasNode({
          x:this.x, y:this.y, rotation: this.rotation
        })
        streak.after(300, streak.removeSelf)
        this.parent.append(streak)
        this.rotation += (Math.random() - 0.5)*(1-(rs * 0.0015))
        this.elapsed += 500
        var dmg = Math.abs(rs * 0.5)
        this.target.health -= dmg
        var ex = new Explosion(0.25) //dmg / 100)
        ex.x = this.x
        ex.y = this.y
        this.parent.append(ex)
        this.fill = 'darkred'
        this.health -= 2
        this.model.height = 1
        if (!this.hasHit) {
          var c = new Circle(10, {x: 30, scale:[0.2,1], strokeWidth:5, fill:'none', stroke:'#ba88ba'})
          if (rs <= 1) rs = 1
          c.animateTo('x', 30+rs*0.03, 300, 'sqrt')
          c.animateToFactor('radius', 2 + rs*0.005, 300, 'sqrt')
          c.animate('opacity', 1, 0, 300, 'sqrt')
          streak.append(c)
        }
        this.hasHit = true
      }

    })



    PointDefenseMissiles = Klass(Weapon, {
      movementSpeed : 250,
      turningSpeed : 3,
      health : 1,
      range : 250,
      optimalRange : 30,
      reloadTime : 500,
      damage : 4,
      hitRadius : 9,
      height: 1,
      salvos: 2,
      color: '#dd2222',
      damageType : 'explosive',

      initAI : function() {
        this.rotation = this.predictAngleToTarget(this.movementSpeed) + (Math.random() - 0.5)
        this.reloadTime *= Math.pow(0.9, this.techLevel)
        this.turningSpeed *= Math.pow(1.2, this.techLevel)
        this.movementSpeed *= Math.pow(1.1, this.techLevel)
        this.after(1400, this.removeSelf)
      },

      ai : function(t, dt) {
        var angle = this.angleTo(this.target)
        this.turnToward( angle )
        this.moveAt(1-Math.sqrt(this.elapsed/6000))
      },

      hit : function() {
        this.target.rotation += Math.random()*0.5 - 0.25
        this.target.movementSpeed *= 1 - (2 / this.target.health)
        this.target.health -= this.damage
        this.health = 0
      }

    })

    PointDefenseGun = Klass(Weapon, {
      movementSpeed : 450,
      turningSpeed : 0,
      health : 1,
      range : 150,
      height : 1,
      optimalRange : 30,
      reloadTime : 20,
      hitRadius : 5,
      damage : 2,
      salvos: 2,
      color: '#dd2222',
      damageType : 'kinetic',

      initAI : function() {
        this.x += Math.cos(this.ship.rotation)*20
        this.y += Math.sin(this.ship.rotation)*20
        this.model.width = 4
        this.hitRadius += this.techLevel
        if (this.techLevel >= 2) {
          this.rotation = this.predictAngleToTarget(this.movementSpeed)
        } else {
          this.rotation = this.angleTo(this.target)
        }
        this.after(600, this.removeSelf)
      },

      ai : function(t, dt) {
        this.moveAt(1-Math.sqrt(this.elapsed/1200))
      },

      hit : function() {
        this.target.rotation += Math.random()*0.5 - 0.25
        this.target.movementSpeed *= 1 - (1 / this.target.health)
        this.target.health -= this.damage
        this.health = 0
      }

    })



    Level = Klass(CanvasNode, {
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
          if (Player.selection.length > 0)
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
          'Start level', function(){
            this.root.dispatchEvent({type: 'started', canvasTarget : this })
          },
          'Back to main menu', function() { this.parentNode.gameOver() }
        )
      },

      query : function(header) {
        var div = E('div', {className : 'message'})
        var msg = new ElementNode(div,
          { x : 320, y : 30, align : 'center' })
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
            { x : 320, y : 30, align : 'center' })
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
          this.query(E('h1', "Level complete"),
            "Next level", function() { this.parentNode.nextLevel() },
            "Play again", function() { this.parentNode.tryAgain() },
            "Back to main menu", function() { this.parentNode.gameOver() }
          )
        })
      },

      createShip : function(team, techLevel, wpn, x, y, noWarp, health) {
        if (!this.ships[team]) this.ships[team] = 0
        var pd
        if (wpn.length) {
          if (wpn[1]) pd = wpn[1]
          wpn = wpn[0]
        }
        if (!pd) {
          switch(wpn) {
            case Missiles: pd = PointDefenseMissiles; break
            case Beam: pd = Beam; break
            case RapidFireRailgun: pd = RapidFireRailgun; break
            default: pd = PointDefenseGun
          }
        }
        var s = new Ship(team,
          new wpn(techLevel), new pd(techLevel),
          x, y, noWarp, health)
        if (team == this.playerTeam) s.strategicAI = Player
        s.when('destroyed', this.destroyedHandler)
        this.ships[team]++
        return s
      },

      createGroup : function(team, techLevel, x, y, weapons, noWarp, health) {
        var i = 0
        var th = this
        var seg = Math.PI*2/(weapons.length-1)
        return weapons.map(function(wpn) {
          if (i == 0) {
            var dx = 0, dy = 0
          } else {
            var angle = i * seg
            var r = Math.max(80, 100 / seg)
            var dx = Math.cos(angle) * r
            var dy = Math.sin(angle) * r
          }
          i++
          return th.createShip(team, techLevel, wpn, x+dx, y+dy, noWarp, health)
        })
      },

      ship : function() {
        this.append(this.createShip.apply(this, arguments))
      },

      shipAfter : function(time, team, techLevel, weapon, x, y, noWarp, health) {
        if (!this.ships[team]) this.ships[team] = 0
        this.ships[team]++
        this.after(time, function() {
          this.ships[team]--
          this.ship(team, techLevel, weapon, x, y, noWarp, health)
        })
      },

      group : function(team, techLevel, x, y, weapons, noWarp, health) {
        this.append.apply(this, this.createGroup.apply(this, arguments))
      },

      groupAfter : function(time, team, techLevel, x, y, weapons, noWarp, health) {
        if (!this.ships[team]) this.ships[team] = 0
        this.ships[team] += weapons.length
        this.after(time, function() {
          this.ships[team] -= weapons.length
          this.group(team, techLevel, x, y, weapons, noWarp, health)
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




    Level1 = Klass(Level, {
      width : 640,
      height : 480,
      scale : 1,

      name : "Run like the wind",
      description : "Try not to get blown up.",

      initialize : function() {
        Level.initialize.call(this)
        this.when('started', function() {
          this.shipAfter(0, this.playerTeam, 0, Missiles, 100, 100)
          this.shipAfter(5000, this.enemyTeam, 0, PointDefenseGun, 680, 300, true)
          this.shipAfter(10000, this.enemyTeam, 0, PointDefenseMissiles, -40, 220, true)
          this.shipAfter(15000, this.enemyTeam, 0, [Beam, PointDefenseGun], 400, 520, true)
        })
      }

    })

    Level2 = Klass(Level, {
      width : 1280,
      height : 960,
      scale : 0.5,

      name : "What doesn't hit you, can't hurt you",
      description : "Take care not to get overwhelmed.",

      initialize : function() {
        Level.initialize.call(this)
        this.when('started', function() {
          this.shipAfter(500, this.playerTeam, 1, Beam, 700, 500)
          this.shipAfter(1000, this.playerTeam, 1, Beam, 600, 300)
          this.notify('Reinforcements are on the way', 20000)
          this.shipAfter(23000, this.playerTeam, 1, Beam, 640, 480)

          this.shipAfter(3000, this.enemyTeam, 0, Missiles, 1320, 800)
          this.shipAfter(17000, this.enemyTeam, 0, Missiles, 1100, -40)
          this.shipAfter(32000, this.enemyTeam, 0, Missiles, 810, 1000)

          this.shipAfter(6000, this.enemyTeam, 0, Missiles, -40, 300)
          this.shipAfter(22000, this.enemyTeam, 0, Missiles, 200, 1000)
          this.shipAfter(24000, this.enemyTeam, 0, PointDefenseGun, 100, 1000)
        })
      }

    })

    Level3 = Klass(Level, {
      width : 1280,
      height : 960,
      scale : 0.5,

      name : "Formation",
      description : "It may be a bit tricky, luck helps.",

      initialize : function() {
        Level.initialize.call(this)
        this.when('started', function() {
          this.ship(this.playerTeam, 2, Railgun, 600, 400)
          this.ship(this.playerTeam, 0, Missiles, 650, 460)
          this.groupAfter(5000, this.enemyTeam, 1, 400, -40, [Beam, Beam])
          this.groupAfter(25000, this.enemyTeam, 0, -40, 520, [Beam, Beam])
          this.shipAfter(15000, this.enemyTeam, 3, Missiles, 1310, 1000)
        })
      }

    })


    Level4 = Klass(Level, {
      name : "Against smaller numbers",
      description : "This should be easy, right?",

      reinforcementMessage : "Reinforcements are on the way.",

      width : 1280,
      height : 960,
      scale : 0.5,

      initialize : function() {
        Level.initialize.call(this)
        this.when('started', function() {
          this.group(this.enemyTeam, 1, 1000, 180,
            [ Beam, Missiles, Railgun ],
            true
          )
          this.groupAfter(2500, this.playerTeam, 1, 200, 800, [
            Missiles,
            PointDefenseMissiles, PointDefenseMissiles,
            PointDefenseGun, PointDefenseGun, PointDefenseGun
          ])
          this.notify(this.reinforcementMessage, 17000)
          this.groupAfter(20000, this.playerTeam, 1, 200, 800,
            [Beam, PointDefenseGun])
        })
      }

    })


    Level5 = Klass(Level, {
      name : "Not fair",
      description : "Well, that's life.",

      width : 1280,
      height : 960,
      scale : 0.5,

      initialize : function() {
        Level.initialize.call(this)
        this.when('started', function() {
          this.groupAfter(3000, this.enemyTeam, 1, this.width * 0.9, this.height * 0.85,
            [ RapidFireRailgun, RapidFireRailgun, RapidFireRailgun ]
          )
          this.group(this.playerTeam, 4, 200, 200, [
            PointDefenseMissiles,
            PointDefenseGun, PointDefenseGun, PointDefenseGun,
            Missiles, Missiles, Missiles
          ])
        })
      }

    })


    Level6 = Klass(Level, {
      name : "Ye Good Ole Slug-Out",
      description : "Tactics, schmactics.",

      width : 1280,
      height : 960,
      scale : 0.5,

      initialize : function() {
        Level.initialize.call(this)
        this.when('started', function() {
          this.group(this.enemyTeam, 0, this.width * 0.9, this.height * 0.85, [
            Railgun, Railgun, Missiles, Beam, RapidFireRailgun
          ])
          this.group(this.playerTeam, 0, this.width * 0.1, this.height * 0.15, [
            Railgun, Railgun, Missiles, Beam, RapidFireRailgun
          ])
        })
      }

    })


    Level7 = Klass(Level, {
      name : "Missiles are awesome",
      description : "And make the computer slow doooown.",

      width : 1600,
      height : 1200,
      scale : 640 / 1600,

      initialize : function() {
        Level.initialize.call(this)
        this.when('started', function() {
          this.group(this.playerTeam, 2, this.width * 0.5, this.height * 0.15, [
            Missiles, Missiles, Missiles, Missiles, Missiles
          ])
          for (var i=1; i<=10; i++) {
            this.groupAfter(i * 3000, this.enemyTeam, 0,
              Math.random() * this.width, this.height + 40,
              [Beam, Beam], true)
          }
        })
      }

    })


    Level8 = Klass(Level, {
      name : "Railguns are awesome too",
      description : "But take oh so long to reload.",

      width : 1600,
      height : 1200,
      scale : 640 / 1600,

      initialize : function() {
        Level.initialize.call(this)
        this.when('started', function() {
          this.group(this.playerTeam, 4, this.width * 0.20, this.height * 0.5, [
            Railgun, Railgun, Railgun, Railgun, Railgun, Railgun
          ])
          for (var i=1; i<=8; i++) {
            this.groupAfter(i * 3000, this.enemyTeam, 4,
              -100, Math.random() * this.height,
              [Missiles, Missiles], true)
          }
          this.groupAfter(9 * 3000, this.enemyTeam, 4,
            this.width*0.5, this.height+50,
            [Missiles, Missiles], true)
          this.groupAfter(10 * 3000, this.enemyTeam, 4,
            this.width*0.5, -50,
            [Missiles, Missiles], true)
        })
      }

    })


    Level9 = Klass(Level, {
      name : "Duels",
      description : "Cheese is awesome as well.",

      width : 1600, height : 1200, scale : 640 / 1600,

      initialize : function() {
        Level.initialize.call(this)
        this.when('started', function() {
          for (var i=1; i<=10; i++) {
            this.groupAfter(i * 3000 - 1500, this.playerTeam, 4,
              this.width * 0.80, this.height * (0.1 + Math.random() * 0.8),
              [ (i <= 6 ? RapidFireRailgun : PointDefenseGun) ])
            this.groupAfter(i * 3000, this.enemyTeam, 4,
              this.width * 0.20, this.height * Math.random(),
              [ RapidFireRailgun ])
          }
        })
      }

    })


    Level10 = Klass(Level, {
      name : "Relaaax",
      description : "An intermission.",

      width : 1600, height : 1200, scale : 640 / 1600,

      initialize : function() {
        Level.initialize.call(this)
        this.when('started', function() {
          this.group(this.playerTeam, 5,
            this.width * 0.5, this.height * 0.5,
            [ RapidFireRailgun, RapidFireRailgun, RapidFireRailgun ])
          for (var i=1; i<=3; i++) {
            this.groupAfter(i * 3000, this.enemyTeam, 2,
              this.width * Math.random(), this.height * Math.random(),
              [ Railgun, PointDefenseMissiles, PointDefenseMissiles ])
          }
          for (var i=5; i<=7; i++) {
            this.groupAfter(i * 3000, this.enemyTeam, 2,
              this.width * Math.random(), this.height * Math.random(),
              [ Beam, PointDefenseMissiles, PointDefenseMissiles ])
          }
          for (var i=9; i<=12; i++) {
            this.groupAfter(i * 3000, this.enemyTeam, 4,
              this.width * Math.random(), this.height * Math.random(),
              [ Missiles, PointDefenseMissiles, PointDefenseMissiles ])
          }
          this.groupAfter(15 * 3000, this.enemyTeam, 5,
            this.width * Math.random(), this.height * Math.random(),
            [ Missiles, Missiles, Missiles, Missiles,
              Missiles, Missiles, Missiles ])
        })
      }

    })


    Level11 = Klass(Level, {
      name : "Encounter",
      description : "Even fight.",

      width : 2000, height : 1500, scale : 640 / 2000,

      initialize : function() {
        Level.initialize.call(this)
        this.when('started', function() {
          this.group(this.playerTeam, 5,
            this.width * 0.2, this.height * 0.8, [
            Railgun, Railgun, Railgun,
            Missiles, Missiles, Missiles,
            Beam, Beam, Beam
          ])
          this.group(this.enemyTeam, 5,
            this.width * 0.8, this.height * 0.2, [
            Railgun, Railgun, Railgun,
            Missiles, Missiles, Missiles,
            Beam, Beam, Beam
          ])
        })
      }

    })


    Level12 = Klass(Level, {
      name : "Skirmish",
      description : "Take out the blue ships.",

      width : 1600, height : 1200, scale : 640 / 1600,
      wave : 20,

      initialize : function() {
        Level.initialize.call(this)
        this.when('started', function() {
          this.ships[this.playerTeam] = 100
          this.ships[this.enemyTeam] = 100
          this.newShips()
          this.every(4000, this.newShips)
        })
      },

      newShips : function(after) {
        if (!after) after = 0
        this.wave--
        if (this.wave == 1) {
          this.ships[this.playerTeam] -= 100
          this.ships[this.enemyTeam] -= 100
        } else if (this.wave == 0) {
          return false
        }
        var friends = this.ships[this.playerTeam] || 0
        var enemies = this.ships[this.enemyTeam] || 0
        var playerWeapons = new Array(Math.max(1, enemies - friends)).
                            map(this.randomWeapon)
        var enemyWeapons = new Array(Math.max(1, friends - enemies)).
                            map(this.randomWeapon)
        this.groupAfter(after, this.playerTeam, Math.random()*6,
          this.width * (0.2 + Math.random() * 0.6),
          this.height * (0.2 + Math.random() * 0.6),
          playerWeapons)
        this.groupAfter(after, this.enemyTeam, Math.random()*6,
          this.width * (0.2 + Math.random() * 0.6),
          this.height * (0.2 + Math.random() * 0.6),
          enemyWeapons)
      },

      weapons : [Beam, Missiles, Railgun],

      randomWeapon : function() {
        if (Math.random() < 0.02) return RapidFireRailgun
        return Level12.weapons.pick()
      }
    })


    Level13 = Klass(Level, {
      name : "Random encounter",
      description : "How will you cope?",

      width : 1600, height : 1200, scale : 640 / 1600,

      initialize : function() {
        Level.initialize.call(this)
        this.when('started', function() {
          var tl = Math.random()*6
          var ships = 3 + Math.floor(Math.random()*7)
          var pl = new Array(ships).map(Level12.randomWeapon)
          var en = new Array(ships).map(Level12.randomWeapon)
          this.group(this.playerTeam, tl,
            this.width * 0.2, this.height * 0.8, pl)
          this.group(this.enemyTeam, tl,
            this.width * 0.8, this.height * 0.2, en)
        })
      }

    })


    MenuLevel = Klass(Level, {
      width : 1280,
      height : 960,
      scale : 0.5,
      playerTeam : null,
      enemyTeam : null,

      initialize : function() {
        Level.initialize.call(this)
        this.menu = new CanvasNode()
        this.menu.scale = 2
        this.menu.zIndex = 100
        this.append(this.menu)
        this.setupMenu()
        this.newShip()
        this.every(4000, this.newShip)
        this.selectRect.opacity = 0
      },

      showDescription : function() {},

      newShip : function() {
        var blue = '#2266aa'
        var red = '#aa2222'
        var blues = this.childNodes.filter(function(n) { return n.team == blue }).length
        var reds = this.childNodes.filter(function(n) { return n.team == red }).length
        var d = 1 + Math.max(0, blues - reds)
        this.taskforce(red, Math.random()*this.width, Math.random()*this.height, d)
        var d = 1 + Math.max(0, reds - blues)
        this.taskforce(blue, Math.random()*this.width, Math.random()*this.height, d)
      },

      taskforce : function(color, x, y, size) {
        var wps = [Missiles, Beam, Railgun]
        var tl = Math.random() * 6
        for (var i=0; i<size; i++) {
          var wp = wps[Math.floor(wps.length * Math.random())]
          var pd = (wp == Missiles) ? PointDefenseMissiles : PointDefenseGun
          if (wp == Beam) pd = Beam
          if (Math.random() < 0.03) {
            wp = pd = RapidFireRailgun
          }
          var ship = new Ship(color, new wp(tl), new pd(tl),
            x + Math.random() * 100 - 50,
            y + Math.random() * 100 - 50)
          this.append(ship)
        }
      },

      setupMenu : function() {
        var elem = E('h1')
        elem.appendChild(T('ELYSIA GENOME EDITOR'))
/*        elem.appendChild(E('span', '+', {style: {color: 'red'}}))
        elem.appendChild(T('FLEET'))*/
        var title = new ElementNode(elem, {
          x: 320, y: 40, zIndex: 1002, align: 'center', cursor: 'default'
        })
        var th = this
        var controls = new CanvasNode()
        var bg = new ElementNode(E('div', {
          style: {
            width : '640px',
            height : '480px',
            backgroundColor: this.bgColor,
            opacity: 0.5
          }
        }), {zIndex : 1001})
        controls.append(bg)
        controls.display = 'none'
        controls.opacity = 0
        var levelList = E('ol')
        MissileFleet.levels.slice(1).forEach(function(lvl, i) {
          var li = E('li', E('h3', (i+1) + '. ' + lvl.prototype.name))
          li.onclick = function(){
            if (th.clicked) return
            th.clicked = true
            th.menu.controls.animateTo('opacity', 0, 300, 'sine')
            th.after(300, function() {
              this.parentNode.jumpToLevel(MissileFleet.levels.indexOf(lvl))
            })
          }
          li.style.cursor = 'pointer'
          levelList.appendChild(li)
        })
        var levelHeader = E('h2', 'JUMP TO LEVEL')
        var jump = new ElementNode(levelHeader, {
          zIndex : 1002,
          x : 320, y : 120,
          align : 'center'
        })
        var levels = new ElementNode(levelList, {
          zIndex : 1002,
          x : 320, y : 164,
          align : 'center'
        })
        var divider = new Rectangle(540, 1, {
          centered: true, x: 320, y: 87.5, fill: 'red'
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



    MissileFleet = Klass(CanvasNode, {
      levelIndex : 0,
      levels : [MenuLevel, Level1, Level2, Level3, Level4, Level5, Level6, Level7, Level8, Level9, Level10, Level11, Level13],

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
        Player.waypoints = {}
        Player.targets = {}
        Player.selection = []
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
      var c = E.canvas(640, 480)
      var d = E('div', { id: 'screen' })
      d.appendChild(c)
      document.body.appendChild(d)
      MF = new MissileFleet(c)
    }
