class Lobe extends Drawable{
  function Lobe(parent:MovieClip,layer:Number) {
     super(parent,layer);
  }
  function onReleaseKey() {
    _root.tf.text="AWECOOL";
  } 
  function onPressKey() {
    _root.tf.text="aWECOOL";
  } 
}