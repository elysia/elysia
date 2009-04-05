import Shape

class Rect extends Shape {
   var mUpperLeft:Point;
   var mLowerRight:Point;
   //returns an array of shapes that is this shape intersected
   function Rect(upperLeft:Point,lowerRight:Point) {
      mUpperLeft=upperLeft;
      mLowerRight=lowerRight;
   }
   function valid() :Boolean{
      return mUpperLeft.x<=mLowerRight.x&&mUpperLeft.y<=mLowerRight.y;
   }
   function cutOutShape(deleteShape):Array  {
       if (deleteShape.typeOf()=="Rect") {
           r0=new Shape(new Point(mUpperLeft.x,mUpperLeft.y),
                        new Point(deleteShape.mLowerRight.x,mLowerRight.y));
           r1=new Shape(new Point(mUpperLeft.x,mUpperLeft.y),
                        new Point(deleteShape.mLowerRight.x,mLowerRight.y));
           if (deleteShape.upperLeft.x<=upperLeft.x
               &&deleteShape.upperLeft.y<=upperLeft.y
               &&deleteShape.lowerRight.x>=lowerRight.x
               &&deleteShape.lowerRight.y>=lowerRight.y) {
              return new Array();
           }
       }      
   }
   function typeOf():String { return "Rect";}
}