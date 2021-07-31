$include<consts.inc>

var info ;
var labmouse ;
var sound ; ;
var animation ;
var sprite ;
var cursor ;
var line ;
var corner ;

function Init(args) {
   system.writeMessage('Game started') ;
   system.showCursor(false) ;

   game.setBackgroundColor(0,0,0) ;

   animation = game.loadAnimation('animation.png',100,100,15,10) ;
     
   sprite = game.loadSprite('sprite.png') ;
   
   corner = game.loadSprite('corner.png') ;
   cursor = game.loadSprite('cursor.png') ;
   cursor.setHotSpot(0,0) ;
   
   info = game.loadText("arial.ttf","",20) ;   
   info.setColor(40,255,255) ;

   labelmouse = game.loadText("arial.ttf","",20) ;

   sound = game.loadSound('signal.wav') ;

   line = game.createLine(255,0,0) ;
      
   return true ;
}
 
function Render() {
   var mouse = game.getMousePos() ;
   
   sprite.renderTo(200,420) ;
   animation.renderTo(500,400) ;

   corner.renderTo(790,10) ;
   corner.renderTo(10,590) ;
   corner.renderTo(790,590) ;
   
   line.drawTo(100,250,700,250) ;
   
   labelmouse.setText("Mouse pos: ["+mouse.x+","+mouse.y+"]") ;
   labelmouse.printTo(20,20) ;
   
   info.setColor(40,255,40) ;
   info.setText("Total time:" +Math.round(game.getTotalTime())) ;
   info.printTo(400,20) ;
   
   info.setColor(40,255,255) ;   
   info.setText("Press F2 to other scene") ;
   info.printTo(20,50) ;
   info.setText("Press LButton to play sound") ;
   info.printTo(20,80) ;
   info.setText("Press Space to start/stop animation") ;
   info.printTo(20,110) ;
   info.setText("Press F3 to rotate sprite") ;
   info.printTo(400,50) ;
   info.setText("Press F4 to scale sprite") ;
   info.printTo(400,80) ;
   info.setText("Press F6 to mirror sprite") ;
   info.printTo(400,110) ;
   
   info.setText("Press Escape to exit") ;
   info.printTo(290,150) ;   
      
   cursor.renderTo(mouse.x,mouse.y) ;
   return true ;

}

function Frame(dt) {

   if (game.isKeyDown(KEY_ESCAPE)) return false ;
   
   if (game.isKeyDown(KEY_SPACE)) {
	 if (animation.isPlayed()) animation.stop() ; else animation.play() ;  
   }

   if (game.isKeyDown(KEY_F2)) game.goToScript(SCENESCRIPT,{text:"Hello"}) ;   
   if (game.isKeyDown(KEY_F3)) sprite.setAngle(45) ;
   if (game.isKeyDown(KEY_F4)) sprite.setScale(150) ;
   if (game.isKeyDown(KEY_F6)) sprite.mirrorHorz(true) ;
   
   if (game.isLeftButtonClicked()) { 
     system.writeMessage('LButton click') ;
     sound.play() ;
   }
      
   return true ;
}
