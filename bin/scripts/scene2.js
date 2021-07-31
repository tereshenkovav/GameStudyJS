var info ;

function Init(args) {
   game.setBackgroundColor(0,0,128) ;
   
   info = game.loadText("arial.ttf","",14) ;
   info.setText("Args: "+args.text+", press F2 to back") ;
   info.setSize(36) ;
   info.setColor(100,255,100) ;

   return true ;
}
 
function Render() {
   info.printTo(200,20) ;
   return true ;
}

function Frame(dt) {

   if (game.isKeyDown(KEY_ESCAPE)) return false ;

   if (game.isKeyDown(KEY_F2)) {
      game.goToScript("main",null) ;
   }

   return true ;
}
