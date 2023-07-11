var cock ;
var gun ;                                                                                                    
var slime ;
var x ;
var y ;
var v ;
var cockexist ;
var text ;
var help ;
var score ;
var v0 ;
var snd_fire ;
var snd_eat ;

function Init() {
   cock = game.loadSprite('cock.png') ;
   gun = game.loadSprite('gun.png') ;
   slime = game.loadSprite('slime.png') ;

   snd_fire = game.loadSound('fire.wav') ;
   snd_eat = game.loadSound('eat.wav') ;

   text = game.loadText("Arial.ttf","Очки: 0",32) ;
   help = game.loadText("Arial.ttf","Нажимайте Пробел,\nчтобы выстрелить\nпетухом в слайма",20) ;
   help.setColor(200,200,200) ;
   system.writeMessage('Игра начата') ;

   game.setGameTitle("Накорми слайма!") ;

   x = 100;
   v0 = 400;
   v = v0 ;
   cockexist = false ;
   score = 0 ;
   
   return true ;
}
 
function Render() {
   slime.renderTo(x,100) ;
   gun.renderTo(400,499) ;
   help.printTo(10,200) ;
   text.printTo(610,230) ;
   if (cockexist) {
	 cock.renderTo(400,y) ;  
   }
   
  return true ;
}

function Frame(dt) {
   x = x + v*dt ;	
   
   // Разворот слайма, когда дошел до правого края
   if (x>700) {
	   v = -v0 ;
   }
   
   // Разворот слайма, когда дошел до левого края
   if (x<100) {
       v = v0;
   }
   
   // Движение петуха вверх, если существует
   if ((y>100)&&(cockexist)) {
	   y = y - 300*dt ;
   }
   
   // Если петух долетел до края, то он исчезает
   if (y<=100) {
	   cockexist = false ;
   }
   
   // Если петух существует
   if (cockexist) {
	   // Если расстояние между слаймом и петухом меньше 100
	   if ((x-400)*(x-400)+(y-100)*(y-100)<100*100) {
		   snd_eat.play() ;
		   cockexist = false ;
		   system.writeMessage('Петух съеден') ;
		   score = score + 1 ;
		   text.setText("Очки: "+score) ;
	   }
   }
   
   // Если нажат пробел и петух не существует
   if ((game.isKeyDown(KEY_SPACE))&&(!cockexist)) {
	   snd_fire.play() ;
	   cockexist = true ;
	   y = 500 ;
   }
   
   return true ;
}

