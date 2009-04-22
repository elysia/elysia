import Gene;
class Shape extends Gene {
   function cutOutShape(deleteShape):Void{}
   function typeOf():String { return "Shape";}
   function isValid():Boolean { return true;}
   function within(point:Point):Boolean {return false;}
   function topLeftCorner():Point {return new Point(0,0);}
   function bottomRightCorner():Point {return new Point(1,1);}
   function scale(zeroPoint:Point,scaleSize:Point):Shape {return this;}
}
