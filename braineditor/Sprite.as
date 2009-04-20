import Point;
//This file should contain all movie clip


class Sprite {
  var mSurface:MovieClip;
  var x:Number;
  var y:Number;
  var height:Number;
  var width:Number;
  static var sNumMovieClips:Number=0;
  function Sprite (parent:MovieClip,depthValue:Number) {
    if (depthValue==null) 
      depthValue=parent.getNextHighestDepth();
    this.mSurface=parent.createEmptyMovieClip(String(sNumMovieClips),depthValue);
    sNumMovieClips++;
    var sprite=this;
    mSurface.onPress=function() { sprite.onPress(); };
    mSurface.onDragOver=function() { sprite.onDragOver(); };
    mSurface.onEnterFrame=function() {sprite.onEnterFrame();};

    mSurface.onKeyDown=function() {sprite.onPressKey();}   ;
    mSurface.onKeyUp=function() {sprite.onReleaseKey();}     ;

    mSurface.onKillFocus=function(newFocus:Object){sprite.onKillFocus(newFocus);};
    mSurface.onRelease=function() {sprite.onRelease();}   ;
    mSurface.onReleaseOutside=function(){sprite.onReleaseOutside();};
    mSurface.onRollOut=function() {sprite.onRollOut();}   ;
    mSurface.onRollOver=function() {sprite.onRollOver();}  ;
    mSurface.onSetFocus=function(oldFocus:Object){sprite.onSetFocus(oldFocus);};
    mSurface.onDragOut=function() {sprite.onDragOut();} ;  
    updateData();
  }
  function onPressKey():Void {
  }
  function onReleaseKey():Void {
  }
  function onPress():Void {
  }
  function onDragOver():Void {     
  }
  function onDragOut():Void {
  }
  function onEnterFrame():Void {     
  }
  function onKillFocus(newFocus:Object):Void {     
  }
  function onSetFocus(oldFocus:Object):Void {     
  }
  function onRelease():Void {
  }
  function onReleaseOutside():Void {
  }
  function onRollOut():Void {
  }
  function onRollOver():Void {
  }
  function worldToLocal(worldPos:Point):Point {
     var pt=worldPos.clone();
     mSurface.globalToLocal(pt);
     return pt;
  }
  function localToWorld(localPos:Point):Point {
     var pt=localPos.clone();
     mSurface.localToGlobal(pt);
     return pt;
  }
  function worldToParent(worldPos:Point):Point {
     var pt=worldPos.clone();
     mSurface._parent.globalToLocal(pt);
     return pt;
  }
  function parentToWorld(localPos:Point):Point {
     var pt=localPos.clone();
     mSurface._parent.localToGlobal(pt);
     return pt;
  }
  function remove():Void {
     mSurface.removeMovieClip();
  }
  function lineStyle(lineThickness,lineColor,lineAlpha) {
    mSurface.lineStyle(lineThickness,lineColor,lineAlpha);
  }
  function drawBoxOutline(topLeft:Point,botRight:Point, lineThickness:Number, lineColor:Number, lineAlpha:Number, fillColor:Number, fillAlpha:Number) {
      lineStyle(lineThickness,lineColor,lineAlpha);
      drawBox(topLeft,botRight,fillColor,fillAlpha);
  }
  function setPosition(pos:Point):Void {
    mSurface._x=pos.x;
    mSurface._y=pos.y;
    updateData();
  }
  function getPosition():Point {
    return new Point(mSurface._x,mSurface._y);
  }
  function drawBoxWorldCoords(topLeft:Point,botRight:Point, color:Number, alpha:Number) {
     var localTopLeft=worldToLocal(topLeft);
     var localBotRight=worldToLocal(botRight);
     drawBox(localTopLeft,localBotRight,color,alpha);
  }
  function drawBox(localTopLeft:Point,localBotRight:Point, color:Number, alpha:Number) {
     mSurface.beginFill(color,alpha);
     mSurface.moveTo(localTopLeft.x,localTopLeft.y);
     mSurface.lineTo(localTopLeft.x,localBotRight.y);
     mSurface.lineTo(localBotRight.x,localBotRight.y);
     mSurface.lineTo(localBotRight.x,localTopLeft.y);
     mSurface.endFill();
  }
  function penTo(worldPos:Point) :Void{
     worldPos=worldToLocal(worldPos);
     mSurface.moveTo(worldPos.x,worldPos.y);
  }
  function drawTo(worldPos:Point):Void {
     worldPos=worldToLocal(worldPos);
     mSurface.lineTo(worldPos.x,worldPos.y);
  }
  function translate(worldVector:Point):Void {
     mSurface._x+=worldVector.x;
     mSurface._y+=worldVector.y;
     updateData();
  }
  function translateTo(worldPos:Point):Void {
     mSurface._x=worldPos.x;
     mSurface._y=worldPos.y;
     updateData();
  }
  function resize(dimensions:Point):Void {
    mSurface._width=dimensions.x;
    mSurface._height=dimensions.y;
    updateData();
  }
  function clear():Void {
    mSurface.clear();
  }
  function updateData():Void {
   this.x=mSurface._x;
   this.y=mSurface._y;
   this.height=mSurface._height;
   this.width=mSurface._width;	
  }
}