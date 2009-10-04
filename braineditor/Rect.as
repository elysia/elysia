import Shape

class Rect extends Shape {
   var mUpperLeft:Point;
   var mLowerRight:Point;
   //returns an array of shapes that is this shape intersected
   function Rect(upperLeft:Point,lowerRight:Point) {
      mUpperLeft=upperLeft.minimum(lowerRight);
      mLowerRight=upperLeft.maximum(lowerRight);
   }
   function topLeftCorner():Point {return mUpperLeft;}
   function bottomRightCorner():Point {return mLowerRight;}
   function isValid() :Boolean{
      return mUpperLeft.x<mLowerRight.x&&mUpperLeft.y<mLowerRight.y;
   }
   function makeValid():Rect  {
     return new Rect(mUpperLeft.minimum(mLowerRight),mUpperLeft.maximum(mLowerRight));
   }
   function intersect(rect:Rect):Rect {
     return new Rect(mUpperLeft.maximum(rect.mUpperLeft),mLowerRight.minimum(rect.mLowerRight));
   }
   function equals(rect:Rect):Boolean {
      return mUpperLeft.x==rect.mUpperLeft.x&&mUpperLeft.y==rect.mUpperLeft.y&&mLowerRight.x==rect.mLowerRight.x&&mLowerRight.y==rect.mLowerRight.y;
   }
   function drawBox(sprite:Sprite, fillColor:Number, fillAlpha:Number) {
       sprite.drawBoxOutline(mUpperLeft,mLowerRight,Colors.invisible,Colors.white,Colors.invisible,fillColor,fillAlpha);
   }
   function toString():String {
      return "["+mUpperLeft.toString()+"::"+mLowerRight+"]";
   }
   
   function cutOutShape(unboundDeleteShape):Array  {
       var a=new Array();
       a.push(unboundDeleteShape);
       if (unboundDeleteShape.typeOf()=="Rect") {
           var deleteShape=new Rect(unboundDeleteShape.mUpperLeft.maximum(mUpperLeft).minimum(mLowerRight),
                                unboundDeleteShape.mLowerRight.minimum(mLowerRight).maximum(mUpperLeft));
           var r0=new Rect(new Point(mUpperLeft.x,mUpperLeft.y),
                        new Point(deleteShape.mUpperLeft.x,mLowerRight.y));
           var r1=new Rect(new Point(deleteShape.mUpperLeft.x,mUpperLeft.y),
                        new Point(deleteShape.mLowerRight.x,deleteShape.mUpperLeft.y));
           var r2=new Rect(new Point(deleteShape.mUpperLeft.x,deleteShape.mLowerRight.y),
                        new Point(deleteShape.mLowerRight.x,mLowerRight.y));
           var r3=new Rect(new Point(deleteShape.mLowerRight.x,mUpperLeft.y),
                        new Point(mLowerRight.x,mLowerRight.y));
           var retval=new Array();

           if (r0.isValid()) {
              retval.push(r0);
           }
           if (r1.isValid()) {
              retval.push(r1);
           }
           if (r2.isValid()) {
              retval.push(r2);
           }
           if (r3.isValid()) {
              retval.push(r3);
           }
           return retval;
       }
   }
   function typeOf():String { return "Rect";}
   function within(point:Point):Boolean {
      return point.x>=mUpperLeft.x&&point.x<=mLowerRight.x&&
             point.y>=mUpperLeft.y&&point.y<=mLowerRight.y;
   }
   function withinShape(rect):Boolean {
      return rect.intersect(this).equals(this);
   }
   function scale(zeroPoint:Point,scaleSize:Point):Shape {
      return new Rect(((mUpperLeft.subtractVector(zeroPoint)).scale(scaleSize)).addVector(zeroPoint),
                      ((mLowerRight.subtractVector(zeroPoint)).scale(scaleSize)).addVector(zeroPoint));
   }
}