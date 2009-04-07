import Shape

class Rect extends Shape {
   var mUpperLeft:Point;
   var mLowerRight:Point;
   //returns an array of shapes that is this shape intersected
   function Rect(upperLeft:Point,lowerRight:Point) {
      mUpperLeft=upperLeft.minimum(lowerRight);
      mLowerRight=upperLeft.maximum(lowerRight);
   }
   function isValid() :Boolean{
      return mUpperLeft.x<mLowerRight.x&&mUpperLeft.y<mLowerRight.y;
   }
   function drawBox(sprite:Sprite, fillColor:Number, fillAlpha:Number) {
       sprite.drawBoxOutline(mUpperLeft,mLowerRight,0,0xffffff,0,fillColor,fillAlpha);
   }
   function toString():String {
      return "["+mUpperLeft.toString()+"-"+mLowerRight+"]";
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
}