import Math;
class Point {

        var x : Number;
        var y : Number;

        function Point( x : Number, y : Number ){
          this.x=x;this.y=y;
        }

        function normalize( length : Number ) : Void {
           var len=Math.sqrt(this.x*this.x+this.y*this.y);
           length/=len;
           this.x*=length;
           this.y*=length;
        }

        function addVector ( p : Point ) : Point{
           return new Point(this.x + p.x, this.y + p.y);
        }
        function subtractVector( p : Point ) : Point {
           return new Point(this.x-p.x,this.y-p.y);
        }
        function equals( p : Object ) : Boolean {
           return p.x==this.x&&p.y==this.y;
        }
        function addOffset( dx : Number, dy : Number ) : Void {
           this.x+=dx;
           this.y+=dx;
        }
        function clone() : Point {
            return new Point(this.x,this.y);
        }
        function toString() : String {
           return String(x)+","+String(y);
        }


        static function distance( p1 : Point, p2 : Point ) : Number {
            var dx=(p1.x-p2.x);
            var dy=(p1.y-p2.y);
            return Math.sqrt(dx*dx+dy*dy);
        }

        static function interpolate( p1 : Point, p2 : Point, f : Number ) : Point {
            return new Point (p1.x*f+p2.x*(1.0-f),p1.y*f+p2.y*(1.0-f));
        }
//        static function polar( dist : Number, angle : Number ) : Point {
//            return new Point (p1.x*f+p2.x*(1.0-f),p1.y*f+p2.y*(1.0-f));
//        }

}