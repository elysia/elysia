    /*
      Elysia Genomics Editor - This editor allows users to select regions and paint them with gene properties

      Copyright (C) 2009  Daniel Reiter Horn, Ilmari Heikkinen

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
    windowWidth=1024;
    windowHeight=768;
    zMin=-99999999999999.;
    DEFAULT_CURSOR = 'default'
    MOVE_TO_CURSOR = 'url(moveto.png) 9 9, move'
    TARGET_CURSOR = 'crosshair'
    SELECT_CURSOR = 'pointer'
    function debugPrint(x) {
        if(typeof(console)!='undefined') {
            if (console) {
                if (console.hasOwnProperty('log')) {
                    console.log(x);
                }
            }
        }
    }

    function onlyOneElement(a) {
        var onlyOneElement=false;
        for (i in a) {
            if (onlyOneElement) {
                onlyOneElement=false;
                break;
            }else {
                onlyOneElement=true;
            }
        }
        return onlyOneElement;
    }
    function distanceSquared(A,B) {
        var a=(A[0]-B[0]);
        var b=(A[1]-B[1]);
        return a*a+b*b;
    }
    function nearCorner(point, boundingBox) {
        var testConstant=3;
        testConstant*=testConstant;
        if (point[0]-testConstant<boundingBox[0]&&point[1]-testConstant<boundingBox[1]) {
            return 0;
        }

        if (point[0]+testConstant>boundingBox[2]&&point[1]-testConstant<boundingBox[1]) {
            return 1;
        }

        if (point[0]+testConstant>boundingBox[2]&&point[1]+testConstant>boundingBox[3]) {
            return 2;
        }

        if (point[0]-testConstant<boundingBox[0]&&point[1]+testConstant>boundingBox[3]) {
            return 3;
        }
        return -1;
    }
    function nearEdge(point, boundingBox) {
        var testConstant=3;
        testConstant*=testConstant;
        if (point[0]-testConstant<boundingBox[0]) {
            return 0;
        }
        if (point[1]-testConstant<boundingBox[1]) {
            return 1;
        }
        if (point[0]+testConstant>boundingBox[2]) {
            return 2;
        }
        if (point[1]+testConstant>boundingBox[3]) {
            return 3;
        }
        return -1;
    }

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
    function IFrame(parentElement)
    {
   // Create the iframe which will be returned
   var iframe = document.createElement("iframe");

   // If no parent element is specified then use body as the parent element
   if(parentElement == null)
      parentElement = document.body;

   // This is necessary in order to initialize the document inside the iframe
   parentElement.appendChild(iframe);

   // Initiate the iframe's document to null
   iframe.doc = null;

   // Depending on browser platform get the iframe's document, this is only
   // available if the iframe has already been appended to an element which
   // has been added to the document
   if(iframe.contentDocument)
      // Firefox, Opera
      iframe.doc = iframe.contentDocument;
   else if(iframe.contentWindow)
      // Internet Explorer
      iframe.doc = iframe.contentWindow.document;
   else if(iframe.document)
      // Others?
      iframe.doc = iframe.document;

   // If we did not succeed in finding the document then throw an exception
   if(iframe.doc == null)
      throw "Document not found, append the parent element to the DOM before creating the IFrame";

   // Create the script inside the iframe's document which will call the
   iframe.doc.open();
   iframe.doc.close();

   // Return the iframe, now with an extra property iframe.doc containing the
   // iframe's document
   return iframe;
    }
    lobeIFrameCount=0;
    LobeIFrame = function(selection) {
        /*
        var ifrm = document.createElement("IFRAME");
        
        ifrm.width="640px";
        ifrm.height="480px";
        ifrm.style.zIndex=10000000;
        ifrm.style.frameborder="1";
        ifrm.style.scrolling="auto";
        */
        
        var iframe = new IFrame(document.body);//E('IFRAME');
        iframe.style.left="100px";
        iframe.style.top="200px";
        iframe.style.position="absolute"
        lobeIFrameCount+=1;
        iframe.id="liframe"+lobeIFrameCount;
        var div = iframe.doc.createElement("div");
        div.id=iframe.id+"d";
        div.style.width = "120px"; div.style.height = "20px";
        
        div.style.border = "solid 0px #00ff00";
        div.style.backgroundColor = "red";
        
//        div.innerHTML = "Hello IFrame!";
        
        iframe.doc.body.appendChild(div);
        bringSelectedIframeToTop(true);
        addHandle(div,iframe);
        var divX = iframe.doc.createElement("divX");
        divX.id=iframe.id+"x";
        divX.style.width = "120px"; div.style.height = "20px";
        div.innerHTML = '<a href="javascript:parent.LobeIFrame.close('+"'"+iframe.id+"'"+')">Close IFrame!</a>';
        addHandle(divX,iframe);
        
        return iframe;
    }
    LobeIFrame.close=function(name) {
        iframe=document.getElementById(name);
        removeHandles(iframe);
        iframe.parentNode.removeChild(iframe);
    }
    Context = function() {
        context={};
        context.selection={};
        context.groupUndoIndexStack=[]
        context.groupRedoIndexStack=[]
        context.mUndoList=[];
        context.mRedoList=[];
        context.mCurUndoCounter=0;
        context.toggleSelect = function(s) {
            if (this.selection.indexOf(s.id) == -1)
                this.select(s);
            else
                this.deselect(s);
        };
        context.frontNonselected = function(sel) {
            var maxZ=zMin;
            var frontObjects=[];
            sel.forEach(function(item) {
                if (item.hasOwnProperty('uid')&&!context.selection[item.uid]) {
                    var zi=item.getZIndex();
                    if (zi>maxZ||frontObjects.length==0) {
                        frontObjects=[item];
                        maxZ=zi;
                    }
                }
            });
            return frontObjects;
        };
        context.frontSelected = function(sel) {
            var maxZ=zMin;
            var frontObjects=[];
            sel.forEach(function(item) {
                if (item.hasOwnProperty('uid')&&context.selection[item.uid]) {
                    var zi=item.getZIndex();
                    if (zi>maxZ||frontObjects.length==0) {
                        frontObjects=[item];
                        maxZ=zi;
                    }
                }
            });
            return frontObjects;
        };
        context.nextItemsInLine = function(sel) {
            var maxZ=zMin;
            var returnAny=true;
            var oldselection=sel;
            sel=new Array(0);
            var item;
            oldselection.forEach(function(item) {
                if(item.hasOwnProperty('uid')) {
                    sel.push(item);
                }
                });
            sel=sel.sort(function(a,b){var az=a.getZIndex(); var bz=b.getZIndex(); if (az==bz) return az.uid<bz.uid;return az>bz;});
            var sellen=sel.length;
            var i=0;
            while (i<sellen) {
                item=sel[i];
                if (context.selection[item.uid]) {
                    returnAny=false;
                    break;
                }
                i+=1;
            }
            if(returnAny) {
                debugPrint("returning any");
                return context.frontNonselected(sel);
            }
            debugPrint("returning before "+i+"/"+sel.length);
            if(i==0) {
                debugPrint("(z="+sel[sel.length-1].getZIndex()+")");
                return [sel[sel.length-1]];
            }
            debugPrint("(z="+sel[i-1].getZIndex()+")");
            return [sel[i-1]];
        };
        context.clearSelection = function() {
            for (var item in this.selection) {
                this.deselect(this.selection[item]);
            }
        };
        context.select = function(s) {
            if (s.hasOwnProperty('uid')) {
                s.root.dispatchEvent({type : 'select', canvasTarget: s});
                this.selection[s.uid]=s;
                s.select();
            }
        };
        context.deselect = function(s) {
            if (s.uid in this.selection) {
                if (s.hasOwnProperty('uid')) {
                    s.root.dispatchEvent({type : 'deselect', canvasTarget: s})
                    delete this.selection[s.uid]
                }
            }
            s.deselect();
        };
        context.getSelectionCenter = function() {
            var x = 0
            var y = 0
            for(var item in this.selection) {
                x += s.x;
                y += s.y;//better centroid detection
            }
            return [ x / this.selection.length, y / this.selection.length ]
        };
        context.setLobeTarget= function(point)  {
            
        };
        context.coalesceUndos = function () {
            if (this.mUndoList.length>1 && this.mRedoList.length>1) {
                var lastUndo=this.mUndoList.pop();
                var penultimateUndo=this.mUndoList.pop();
                var lastRedo=this.mRedoList.pop();
                var penultimateRedo=this.mRedoList.pop();
                this.performedAction(function(){penultimateRedo();lastRedo();},function(){lastUndo();penultimateUndo();});
            }
        };
        
        context.performedAction= function(redoFunction,undoFunction) {
            if (this.mCurUndoCounter!=this.mUndoList.length) {
                var leng=this.mUndoList.length;
                var rleng=this.mRedoList.length;
                var count=0;
                for (var i=this.mCurUndoCounter;i<leng;i+=1) {
                    //console.log ("Pushing redo "+(leng-1-count)+ " to undo stack");
                    this.mUndoList.push(this.mRedoList[leng-1-count]);
                    this.mRedoList.push(this.mUndoList[leng-1-count]);
                    count+=1;
                }
            }
            this.mUndoList.push(undoFunction);
            this.mRedoList.push(redoFunction);
            this.mCurUndoCounter=this.mUndoList.length;
            //console.log ("undo list len "+this.mCurUndoCounter+" redo "+this.mRedoList.length);
        }
        context.undo= function () {      
            if (this.mCurUndoCounter>0) {
                this.mCurUndoCounter--;
                this.mUndoList[this.mCurUndoCounter]();
                // console.log(this.mCurUndoCounter);
            }
        }
        context.redo = function() {
            if (this.mCurUndoCounter<this.mUndoList.length) {
                this.mRedoList[this.mCurUndoCounter]();
                this.mCurUndoCounter++;
            }
        }
        return context;
    }
  
    currentMaxZ = 2;
    
    getUID= (function() {
            var start=0;
            return function() {
                start+=1;
                return start;
            };
        })();

    Selectable = Klass (CanvasNode, {
        initialize: function(editor) {
            CanvasNode.initialize.call(this);
            this.uid=getUID()
            this.mSelected=false;
            this.mEditor=editor;
            {
                var th=this;
                this.addEventListener('mousedown',function(ev) {
                        if (editor.clickedSelected(ev)) {
                            editor.enableMoveOnMouseUp=true;
                            editor.mouseDragDownHandler.bind(editor)(ev);
                        }else {
                            editor.enableMoveOnMouseUp=false;
                            editor.mouseDownHandler.bind(editor)(ev);
                        }
                    },false);
                this.addEventListener('drag',function(ev) {
                        if(editor.enableMoveOnMouseUp) {
                            editor.mouseDragMoveHandler.bind(editor)(ev);
                        }else {
                            editor.mouseDragHandler.bind(editor)(ev);
                        }
                    },false);
            }
        },
        getBoundingBox:function() {
            return CanvasNode.getBoundingBox.call(this);
        },
        select:function(){
            this.mSelected=true;
        },
        deselect:function(){
            this.mSelected=false;
        },
        isSelected:function() {
            return this.mSelected;
        },
        getZIndex: function() {
            return zMin;
        },
        getOrigin:function() {
            var retval=new Array(3);
            retval[0]=this.x;
            retval[1]=this.y;
            retval[2]=this.getZIndex();
            return retval;
        },
        setOrigin:function(origin) {
            this.x=origin[0];
            this.y=origin[1];
        },
        setBoundingBox:function(bb) {
            this.x=bb[0];
            this.y=bb[1];
        }
    });

    Lobe = Klass(Selectable, {
            initialize: function(editor) {
                Selectable.initialize.bind(this)(editor);
                this.lobe=new Rectangle(64,32, {
                        stroke : false,
                        stroke : [0,0,0,0],
                        fill : [128,128,128,0.25],
                        visible : true,
                        zIndex : currentMaxZ+=2
                    });
                this.lobe.x+=5;
                this.lobe.y+=274;
                this.append(this.lobe)
            },
            getZIndex: function() {
                return this.lobe.zIndex;
            },
            getBoundingBox: function() {
                var bb=this.lobe.getBoundingBox();
                var retval=new Array(4);
                retval[0]=bb[0]+this.lobe.x;
                retval[1]=bb[1]+this.lobe.y;
                retval[2]=bb[2]+this.lobe.x;
                retval[3]=bb[3]+this.lobe.y;
                return retval;
            },
            select:function() {
                Selectable.select.call(this);
                this.lobe.fill[3]=0.5;
            },
            deselect:function() {
                Selectable.deselect.call(this);
                this.lobe.fill[3]=0.25;
            },
            getOrigin:function() {
                var retval=new Array(3);
                retval[0]=this.lobe.x;
                retval[1]=this.lobe.y;
                retval[2]=this.getZIndex();
                return retval;
            },
            setOrigin:function(origin) {
                this.lobe.x=origin[0];
                this.lobe.y=origin[1];
                this.lobe.zIndex=origin[2];
            },
            setBoundingBox:function(bb) {
                var x1 = Math.min(bb[0], bb[2])
                var x2 = Math.max(bb[0], bb[2])
                var y1 = Math.min(bb[1], bb[3])
                var y2 = Math.max(bb[1], bb[3])

                this.lobe.x=x1;
                this.lobe.y=y1;
                this.lobe.width=x2-x1;
                this.lobe.height=y2-y1;
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
        this.ignoreNextClick=false;
        var selectionStart, startX, startY, selectionDragPlace, nearWhichEdge=-1, nearWhichCorner=-1;
        var th = this
        var objectsInside = function(rect, mouseUpPoint, isSelectionBox) {
          var x1 = Math.min(rect.cx, rect.x2)
          var x2 = Math.max(rect.cx, rect.x2)
          var y1 = Math.min(rect.cy, rect.y2)
          var y2 = Math.max(rect.cy, rect.y2)
          if (isSelectionBox) {
              if (mouseUpPoint[0]==0&&mouseUpPoint[1]==0) {
                  var delta=20;
                  if (x1-delta<0)
                      x1=0;
                  if (x2+delta>windowWidth)
                      x2=windowWidth;
                  if (y1-delta<0)
                      y1=0;
                  if (y2+delta>windowHeight) 
                      y2=windowHeight;
                  //console.log ("Unknown location guessing ("+x1+","+y1+"),("+x2+","+y2+")");
              }
              return th.childNodes.filter(function(s) {
                      try {
                          var bb=s.getBoundingBox();
                          if (bb[0]<bb[2]&&bb[1]<bb[3]) {
                              var minbb=[Math.max(bb[0],x1),
                                         Math.max(bb[1],y1),
                                         Math.min(bb[2],x2),
                                         Math.min(bb[3],y2)];
                              if (s.hasOwnProperty('uid')&&minbb[0]==bb[0]&&minbb[1]==bb[1]&&minbb[2]==bb[2]&&minbb[3]==bb[3]) {
                                  return true;
                              }else {
                                  return false;
                              }
                          }else return false;
                      }catch(e){return false;}
                  });
          }else {
              x1=x2=mouseUpPoint[0];
              y1=y2=mouseUpPoint[1];
              return th.childNodes.filter(function(s) {
                      try {
                          var bb=s.getBoundingBox();
                          if (bb[0]<bb[2]&&bb[1]<bb[3]) {
                              return x1>=bb[0]&&x1<=bb[2]&&y1>=bb[1]&&y1<=bb[3];
                          }else return false;
                      }catch(e){return false;}
                  });
          }
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
        this.clickedSelected=function(ev) {
          var point = CanvasSupport.tMatrixMultiplyPoint(
            CanvasSupport.tInvertMatrix(this.currentMatrix),
            this.root.mouseX, this.root.mouseY);
          return context.frontSelected(objectsInside({cx:point[0],cy:point[1],x1:point[0],x2:point[1]},point,false)).length!=0;
        };
        this.mouseDragDownHandler=function(ev) {
          th.performedDrag=false;
          var point = CanvasSupport.tMatrixMultiplyPoint(
            CanvasSupport.tInvertMatrix(this.currentMatrix),
            this.root.mouseX, this.root.mouseY
          )
          nearWhichEdge=-1;
          nearWhichCorner=-1;
          var resizePossibility=onlyOneElement(context.selection);
          if (resizePossibility) {
              for (uid in context.selection) {
                  var bounds=context.selection[uid].getBoundingBox();
                  nearWhichCorner=nearCorner(point,bounds);
                  nearWhichEdge=nearEdge(point,bounds);
              
              }              
          }
          selectionStart = point;
          selectionDragPlace = point
        }
        this.mouseDragMoveHandler=function(ev) {
          th.performedDrag=true;
          var point = CanvasSupport.tMatrixMultiplyPoint(
            CanvasSupport.tInvertMatrix(this.currentMatrix),
            this.root.mouseX, this.root.mouseY
          )
          var delta=[point[0]-selectionDragPlace[0],point[1]-selectionDragPlace[1]];
          for (uid in context.selection) {
              var origin=context.selection[uid].getOrigin();
              if (nearWhichCorner!=-1) {
                  var bb=context.selection[uid].getBoundingBox();
                  if (nearWhichCorner==0) {
                      bb[0]+=delta[0];
                      bb[1]+=delta[1];                      
                  }else if (nearWhichCorner==1){
                      bb[2]+=delta[0];
                      bb[1]+=delta[1];
                  }else if (nearWhichCorner==2) {
                      bb[2]+=delta[0];
                      bb[3]+=delta[1];
                  }else if (nearWhichCorner==3) {
                      bb[0]+=delta[0];
                      bb[3]+=delta[1];
                  }
                  context.selection[uid].setBoundingBox(bb);
              }else if (false&&nearWhichEdge!=-1) {
                  //IF YOU ADD THIS EDGE FUNCTION MAKE SURE TO ADD THE CORRESPONDING UNDO BELOW
                  debugPrint("dragEdge "+nearWhichEdge);
              }else {
                  origin[0]+=delta[0];
                  origin[1]+=delta[1];
                  context.selection[uid].setOrigin(origin);
              }
          }
          selectionDragPlace=point;
        }
        this.mouseDownHandler=function(ev) {
          th.performedDrag=false;
          th.enableMoveOnMouseUp=false;
          var point = CanvasSupport.tMatrixMultiplyPoint(
            CanvasSupport.tInvertMatrix(this.currentMatrix),
            this.root.mouseX, this.root.mouseY
          )
          nearWhichEdge=-1;
          nearWhichCorner=-1;
          startX = this.root.mouseX
          startY = this.root.mouseY
          selectionStart = point
          th.selectRect.x2 = th.selectRect.cx = point[0]
          th.selectRect.y2 = th.selectRect.cy = point[1]
        };
        this.mouseDragHandler=function(ev) {
          th.performedDrag=true;
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
        };
        this.bg.addEventListener('mousedown', this.mouseDownHandler, false)
        this.bg.addEventListener('drag', this.mouseDragHandler, false)
        this.makeSelectionMoveUndo=function(delta) {
            var firstItem=true;
            if (nearWhichCorner!=-1){
              for (uid in th.context.selection) {                
                  var item=context.selection[uid];
                  var newbb=item.getBoundingBox();
                  var oldbb=item.getBoundingBox();
                  if (nearWhichCorner==0) {
                      oldbb[0]-=delta[0];
                      oldbb[1]-=delta[1];                      
                  }else if (nearWhichCorner==1){
                      oldbb[2]-=delta[0];
                      oldbb[1]-=delta[1];
                  }else if (nearWhichCorner==2) {
                      oldbb[2]-=delta[0];
                      oldbb[3]-=delta[1];
                  }else if (nearWhichCorner==3) {
                      oldbb[0]-=delta[0];
                      oldbb[3]-=delta[1];
                  }
                  (function(nBb,oBb,cItem) {
                      var heapNewBb=nBb.slice(0);
                      var heapOldBb=oBb.slice(0);
                      var heapItem=cItem;
                      undoFunction=function() {
                          heapItem.setBoundingBox(heapOldBb);
                      };
                      redoFunction=function() {
                          heapItem.setBoundingBox(heapNewBb);
                      };
                      th.context.performedAction(redoFunction,undoFunction);                    
                  })(newbb,oldbb,item);
                  if (!firstItem) {
                      th.context.coalesceUndos(); 
                  }
                  firstItem=false;                  
              }
            }else if (false&&nearWhichEdge!=-1) {
                  //IF YOU ADD THIS EDGE UNDO FUNCTION MAKE SURE TO ADD THE CORRESPONDING RESIZER ABOVE
            }else {
              for (uid in th.context.selection) {                
                var item=th.context.selection[uid];
                var newOrigin=item.getOrigin();
                var oldOrigin=item.getOrigin();
                for (var i=0;i<delta.length&&i<newOrigin.length;i+=1) {
                    oldOrigin[i]=newOrigin[i]-delta[i];
                }
                (function(nOrigin,oOrigin,cItem) {
                    var heapNewOrigin=nOrigin.slice(0);
                    var heapOldOrigin=oOrigin.slice(0);
                    var heapItem=cItem;
                    undoFunction=function() {
                        heapItem.setOrigin(heapOldOrigin);
                    };
                    redoFunction=function() {
                        heapItem.setOrigin(heapNewOrigin);
                    };
                    th.context.performedAction(redoFunction,undoFunction);                    
                })(newOrigin,oldOrigin,item);
                if (!firstItem) {
                    th.context.coalesceUndos(); 
                }
                firstItem=false;
              }
            }
        }
        this.mouseupHandler = function(ev) {
          var point = CanvasSupport.tMatrixMultiplyPoint(
            CanvasSupport.tInvertMatrix(th.currentMatrix),
            th.root.mouseX, th.root.mouseY
          )
          if(th.enableMoveOnMouseUp&&th.performedDrag) {
              if (selectionStart) {
                  th.mouseDragMoveHandler(ev);
                  th.makeSelectionMoveUndo([point[0]-selectionStart[0],point[1]-selectionStart[1]]);
                  selectionStart=null;
              }
          }else {
            var doIgnoreNext=false;
          //console.log("click at "+JSON.stringify(point)+" selected started at "+JSON.stringify(selectionStart)+ " ignored? "+th.ignoreNextClick+" visible{"+th.selectRect.visible+"}");
            if (selectionStart||!th.ignoreNextClick) {
              var selectionBox=th.performedDrag;
              doIgnoreNext=true;//somehow we get 2 events for every legitimate event. This mitigates that factor.
              th.selectRect.visible = false
              selectionStart = null
              var selection = objectsInside(th.selectRect,point,selectionBox)
              if (ev.shiftKey) {
                if (!selectionBox) {
                    selection=context.frontNonselected(selection);
                }
                selection.forEach(context.select.bind(context));
              } else if (ev.altKey) {
                  if(!selectionBox) {
                      selection=context.frontSelected(selection);
                  }
                  selection.forEach(context.deselect.bind(context));
              } else {
                  if (!selectionBox) {
                      selection=context.nextItemsInLine(selection);
                  }
                  context.clearSelection();
                  selection.forEach(context.select.bind(context));
              }
            } else if (selectionStart && (ev.canvasTarget == th.selectRect || ev.canvasTarget == th.bg)) {
                context.setLobeTarget(point)
              th.selectRect.visible = false
              selectionStart = null
            }
            th.ignoreNextClick=doIgnoreNext; 
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
         var lobe = new Lobe(this);
         var thus = this;
         var isSelected=lobe.selected;
         var context=this.context;
         redoFunction=function(){thus.append(lobe);if (isSelected) {context.select(lobe);}}
         undoFunction=function(){if(lobe.isSelected()) {isSelected=true;}context.deselect(lobe);thus.remove(lobe);}
         redoFunction();
         this.context.performedAction(redoFunction,undoFunction);
      },
      lobeProperties: function() {
          ifrm=new LobeIFrame(this.context.selection)
          
      },
      undo : function () {
          this.context.undo();
      },
      redo : function () {
          this.context.redo();
      },
      beginEditing : function() {
          this.root.dispatchEvent({type: 'started', canvasTarget : this })
      },
      showDescription : function() {
        var desc = E('div')
        desc.appendChild(E('h1', this.name))
        desc.appendChild(E('div', this.description))
        this.query(desc,
          'Begin editing', this.beginEditing,
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
        this.menu.zIndex = 1002
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

      initialize : function(canvasElem, devMode) {
        CanvasNode.initialize.call(this)
        this.canvas = new Canvas(canvasElem)
        this.canvas.frameDuration = 30
        this.canvas.append(this)
        this.canvas.fixedTimestep = true
        this.canvas.clear = false
        this.returnToMenu()
        this.setupEtc()
        if (devMode) {
            this.newEditorState(1);
        }
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
      lobeProperties:function(s) {
         this.currentEditor().lobeProperties()
      },
      undo:function(s) {
         this.currentEditor().undo()
      },
      redo:function(s) {
         this.currentEditor().redo()
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
            ['lobeProperties','function'],
            ['redo','function'],
            ['undo','function'],
          ]
        })
        this.controlPanel.show()
      }

    })



    function init() {
      var c = E.canvas(windowWidth, windowHeight)
      var d = E('div', { id: 'screen' })
      d.appendChild(c)
      document.body.appendChild(d)
      var devmode=(window.location.search.split("devmode=true").length>1);
      GE = new GenomeEditor(c,devmode)
    }
