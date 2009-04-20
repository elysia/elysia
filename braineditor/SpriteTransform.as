class SpriteTransform{
 var x:Number;
 var y:Number;
 var width:Number;
 var height:Number;
 function SpriteTransform() {
   x=0;
   y=0;
   width=1;
   height=1;
 }
 function save(sprite:Sprite):Void {
   x=sprite.mSurface._x;
   y=sprite.mSurface._y;
   width=sprite.mSurface._width;
   height=sprite.mSurface._height;
 }
 function restore(sprite:Sprite):Void {
/*
  sprite.mSurface._x=sprite.x;
  sprite.mSurface._y=sprite.y;
  if (sprite.width)
    sprite.mSurface._width=sprite.width;
  if (sprite.height)
    sprite.mSurface._height=sprite.height;
*/
   sprite.mSurface._x=x;
   sprite.mSurface._y=y;
   if (width) {
     sprite.mSurface._width=width;
   }
   if (height) {
    sprite.mSurface._height=height;
   }

 }
 function toString():String{
  return "["+String(x)+","+String(y)+"]("+String(width)+","+String(height)+")";
 }
}