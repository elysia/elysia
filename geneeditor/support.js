/*
  CAKE - Canvas Animation Kit Experiment

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

/**
  Walks an object obj and returns an indented source code string that can be
  evaled.

  @param obj The object to turn into its source representation.
  @param {Integer} indent Indentation level.
  @param {String} indentCharacter Indentation string.
  @return The source code for the object.
  @type String
  */
Object.toSource = function(obj, indent, indentCharacter) {
  if (indent == null)
    indent = 0
  if (!indentCharacter)
    indentCharacter = '  '
  var indentString = ''
  for (var i = 0; i<indent; i++)
    indentString += indentCharacter
  var str
  if (obj == null) {
    str = 'null'
  } else if (typeof(obj) == 'function') {
    str = Object.prettyFunctionString(obj.toString()).replace(/\n/g, '\n' + indentString)
  } else if (typeof(obj) == 'string') {
    str = obj.escape()
  } else if (obj instanceof Array) {
    var srcs = obj.map(function(e) {
      return Object.toSource(e)
    })
    if (srcs.join(", ").length < 80) {
      str = '[ ' + srcs.join(", ") + ' ]'
    } else {
      var srcs = obj.map(function(e) {
        return Object.toSource(e, indent + 1)
      })
      str = '[\n' + srcs.join(",\n") + '\n' + indentString + ']'
    }
  } else if (typeof(obj) == 'object') {
    var members = []
    for (var i in obj) {
      if (i.search(/^[a-zA-Z0-9]+$/) != -1)
        name = indentString + indentCharacter + i
      else
        name = Object.toSource(i, indent + 1)
      var s = name + ' : ' +
              Object.toSource(obj[i], indent + 1).strip()
      if (typeof(obj[i]) == 'function')
        s = '\n' + s
      members.push(s)
    }
    str = '{\n' +
          members.join(",\n") + '\n' +
          indentString + '}'
  } else {
    str = obj.toString()
  }
  return indentString + str
}

// a horrible horrible hack to prettify function.toString() output
Object.prettyFunctionString = function(str) {
  var nstr = str.replace(/\n\s+/g, '\n')
  var istr = ''
  var inString = false
  var inFor = false
  for(var i = 0; i<nstr.length; i++) {
    var c = nstr[i]
    if (c == '"') {
      inString = !inString
    } else if (inString && c == '\\') {
      i++
    } else if (!inString) {
      if (c == '(') {
        inFor = nstr.slice(i-4,i-1) == 'for'
        istr += '  '
      } else if (c == ')') {
        if (nstr[i-istr.length-1] == '\n') {
          nstr = nstr.splice(i-2, 2, '')
          i -= 2
        }
        istr = istr.slice(2)
      } else if (c == '[') {
        istr += '  '
      } else if (c == ']') {
        istr = istr.slice(2)
      } else if (c == '{' && nstr[i-1] != '\\') {
        inFor = false
        istr += '  '
        nstr = nstr.splice(i+1, 0, '\n'+istr)
        i += istr.length + 1
      } else if (c == '}' && nstr[i-1] != '\\') {
        if (nstr[i-istr.length-1] == '\n') {
          nstr = nstr.splice(i-2, 2, '')
          i -= 2
        } else if (nstr[i-1] == ' '){
          nstr = nstr.splice(i-1, 1, '')
          i -= 1
        } else {
          nstr = nstr.splice(i, 0, '\n'+istr.slice(2))
          i += istr.length - 1
        }
        istr = istr.slice(2)
        if (nstr[i+1] != ';' && nstr.slice(i+1).search(/^\s+else\s+/) == -1) {
          nstr = nstr.splice(i+1, 0, '\n'+istr)
          i += istr.length + 1
        }
      } else if (c == ';') {
        if (!inFor) {
          var idx = nstr.slice(i+1).search(/\S/)
          if (idx > 0) {
            nstr = nstr.splice(i+1, idx, '')
          }
          nstr = nstr.splice(i+1, 0, '\n'+istr)
          i += istr.length + 1
        }
      } else if (c == '\n') {
        nstr = nstr.splice(i+1, 0, istr)
        i += istr.length
      } else if (c == ',' && nstr.slice(i).search(/^,\s+\S+:\s/) != -1) {
        var idx = nstr.slice(i+1).search(/\S/)
        if (idx > 0) {
          nstr = nstr.splice(i+1, idx, '')
        }
        nstr = nstr.splice(i+1, 0, '\n'+istr)
        i += istr.length + 1
      }
    }
  }
  return nstr.replace(/\n+(\s+\n+)?/g, "\n")
}

/**
  GuiConfig creates a DOM control panel for an object.

  new GuiConfig({

    object : myObject, // the object the control panel controls

    container : controlPanelElement, // the DOM element that the control
                                    // panel is attached to when calling
                                    // showControls

    controls : [       // list of controls for the object, required
      // control entries are [method_name, type[, config]]
      ['showText', 'boolean'],
      ['strokeWidth', '1..10'],
      ['radius', '1..1000', {title: 'Circle radius'}]
    ]

  })

*/
GuiConfig = Klass({

  object : null,
  container : null,
  controls : null,
  title : null,
  description : null,

  initialize : function(config) {
    Object.extend(this, config)
    if (!this.container)
      this.container = document.body
    if (!this.controls)
      throw("You need to provide a list of controls.")
    this.element = E('div')
    this.element.className = 'GuiConfig'
    if (this.title)
      this.element.appendChild(E('h3', this.title, {className: 'GuiConfigTitle'}))
    if (this.description)
      this.element.appendChild(E('div', this.description, {className: 'GuiConfigDescription'}))
    this.createControls()
  },

  createControls : function() {
    for (var i=0; i<this.controls.length; i++) {
      var c = this.controls[i]
      if (typeof(c) == 'object' && c.length != null) {
        var config = this.parseType(c[0], c[1], c[2])
      } else if (typeof(c) == 'string') {
        var type = this.guessObjectType(this.object[c])
        var config = this.parseType(c, type, null)
      } else {
        throw("Unsupported control object type: " + c)
      }
      var widget = GuiConfig.widgets[config.type].apply(
        GuiConfig.widgets, config.arguments
      )
      this.element.appendChild(widget)
    }
  },

  guessObjectType : function(object) {
    var t = typeof(object)
    return t
  },

  parseType : function(varName, typeString, config) {
    var cfg = {
      varName : varName,
      methodName : this.parseMethodName(varName),
      object : this,
      title : this.parseTitle(varName)
    }
    if (config)
      Object.extend(cfg, config)
    var type = typeString
    if (typeof typeString == 'string') {
      if (typeString.match(/\.\./)) {
        var sb = typeString.split("..")
        var start = sb[0]
        var end = sb[1]
        var num_re = /[-+]?\d+(\.\d+)?/
        if (start.match(num_re) && end.match(num_re)) {
          if (start.match(/\./) || end.match(/\./)) {
            type = 'float'
            cfg.values = [parseFloat(start), parseFloat(end)]
          } else {
            type = 'int'
            cfg.values = [parseInt(start), parseInt(end)]
          }
        }
      }
    } else if (typeof typeString == 'object') {
      if (typeString.length) {
        type = 'array'
        cfg.values = typeString
      }
    }
    return {
      type: type,
      arguments: [cfg]
    }
  },

  parseTitle : function(varName) {
    var a = varName.split(/(?=[A-Z])/)
    for (var i=1; i<a.length; i++) {
      a[i] = a[i].toLowerCase()
    }
    a[0] = a[0].capitalize()
    return a.join(" ")
  },

  parseMethodName : function(varName) {
    return 'set' + varName.capitalize()
  },

  show : function() {
    if (this.container)
      this.container.appendChild(this.element)
  },

  hide : function() {
    if (this.element.parentNode)
      this.element.parentNode.removeChild(this.element)
  }
})
GuiConfig.widgets = {
  __setValue : function(obj, methodName, varName, value) {
    if (obj.object)
      if (obj.object[methodName])
        obj.object[methodName](value)
      else
        obj.object[varName] = value
  },

  'function' : function(config) {
    var obj = config.object
    var varName = config.varName
    var title = config.title
    var p = E('p')
    var e = E('input', {type:'submit', value: title})
    p.appendChild(e)
    e.onclick = function(ev) {
      if (ev.preventDefault)
        ev.preventDefault()
      obj.object[varName]()
      return false
    }
    return p
  },

  array : function(config) {
    var obj = config.object
    var methodName = config.methodName
    var varName = config.varName
    var val = obj.object && obj.object[varName]
    var title = config.title
    var p = E('p')
    var te = E('span', title)
    te.style.marginBottom = '4px'
    var e = E('select')
    for (var i=0; i<config.values.length; i++) {
      var o = E('option', {
        value : config.values[i],
        content : config.values[i],
        selected : val == config.values[i]
      })
      e.appendChild(o)
    }
    p.appendChild(te)
    p.appendChild(E('br'))
    p.appendChild(e)
    e.onchange = function() {
      GuiConfig.widgets.__setValue(obj, methodName, varName, this.value)
    }
    return p
  },

  boolean : function(config) {
    var obj = config.object
    var methodName = config.methodName
    var varName = config.varName
    var title = config.title
    var p = E('p')
    var e = E('input', {type:'checkbox'})
    p.appendChild(e)
    p.appendChild(E('span',title))
    e.onchange = function() {
      GuiConfig.widgets.__setValue(obj, methodName, varName, this.checked)
    }
    if (obj.object)
      e.checked = obj.object[varName]
    return p
  },

  float : function(config) {
    var start = config.values[0]
    var end = config.values[1]
    return this.slider(function(d) {
      return (1-d)*start + d*end
    }, config)
  },

  int : function(config) {
    var start = config.values[0]
    var end = config.values[1]
    return this.slider(function(d) {
      return parseInt((1-d)*start + d*end)
    }, config)
  },

  slider : function(valueFunction, config) {
    var obj = config.object
    var methodName = config.methodName
    var varName = config.varName
    var title = config.title
    var p = E('p')
    var canvas = E.canvas(200,10)
    p.appendChild(canvas)
    p.appendChild(E('span', title))
    var c = new Canvas(canvas, {redrawOnlyWhenChanged : true})
    var slider = new CanvasNode()
    var bar = new Rectangle(196, 2)
    bar.x = 2
    bar.y = 4
    bar.stroke = 'rgba(0,0,0,0.5)'
    bar.fill = 'rgba(255,255,255,0.5)'
    var knob = new Rectangle(20,10)
    knob.x = 178
    knob.y = 0
    knob.zIndex = 2
    knob.stroke = 'rgba(0,0,0,0.5)'
    knob.fill = 'rgba(255,255,255,0.5)'
    slider.append(bar,knob)
    var x
    var value = 1.0
    var down = false
    canvas.addEventListener('mousedown', function(e) {
      var xy = Mouse.getRelativeCoords(this,e)
      x = xy.x
      down = true
      value = (x - 12) / 176
      value = Math.min(1.0, Math.max(0.0, value))
      knob.x = value * 176 + 2
      var rv = valueFunction(value)
      GuiConfig.widgets.__setValue(obj, methodName, varName, rv)
      c.changed = true
    }, false)
    canvas.addEventListener('mousemove', function(e) {
      var xy = Mouse.getRelativeCoords(this,e)
      if (down && x != null) {
        var dx = xy.x - x
        var d = dx / 176
        value = Math.min(1.0, Math.max(0.0, value+d))
        knob.x = value * 176 + 2
        var rv = valueFunction(value)
        GuiConfig.widgets.__setValue(obj, methodName, varName, rv)
        c.changed = true
      }
      x = xy.x
    }, false)
    var mu = function(e) {
      down = false
    }
    window.addEventListener('mouseup', mu, false)
    canvas.addEventListener('DOMNodeRemovedFromDocument', function(){
      window.removeEventListener('mouseup', mu, false)
    }, false)
    c.append(slider)
    return p
  }
}
