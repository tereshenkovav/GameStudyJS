var rar ;
var rar_x ;
var rar_y ;
var v0 ;
var vd ;
var dir ;
var help ;
var snd_eat ;
var snd_jump ;
var diamonds = new Array() ;
var spr = new Array() ;
var nextdiamond ;
var textleft;
var textscore;
var textcost = new Array() ;
var textenergy;
var timeleft ;
var score ;
var energy ;
var venergy ;
var jump_pause ;

const DIAMOND_COUNT=4 ;
const MINPERIOD=0.33 ;

const DIR_LEFT = 'L' ;
const DIR_WAIT = '0' ;
const DIR_RIGHT = 'R' ;
const JUMP_ENERGY = 100 ;
const BORDERSPACE = 50 ;

var diamond_cost = [1,2,4,8] ;

function setDir(newdir) {
  dir = newdir ;
  if (dir==DIR_LEFT) rar.mirrorHorz(false) ;
  if (dir==DIR_RIGHT) rar.mirrorHorz(true) ;
}

function getRandomInt(min, max){
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

function isDiamondOverRar(diamond) {
  if (jump_pause>0) return false ;
  return (diamond.x-rar_x)*(diamond.x-rar_x)+
         (diamond.y-rar_y)*(diamond.y-rar_y)<(32+16)*(32+16) ;
}

function Init() {
   
   rar = game.loadSprite('rar.png') ;
   spr[0] = game.loadSprite('diamond1.png') ;
   spr[1] = game.loadSprite('diamond2.png') ;
   spr[2] = game.loadSprite('diamond3.png') ;
   spr[3] = game.loadSprite('diamond4.png') ;   

   snd_eat = game.loadSound('eat.wav') ;
   snd_jump = game.loadSound('jump.wav') ;
   
   help = game.loadText("Arial.ttf","Набирайте очки, ловя кристаллы 1 минуту\nСтоимость кристаллов в подсказке слева\nLeft/Right - запуск движения\nDown - остановка\nПробел - телепортация",18) ;
   textleft = game.loadText("Arial.ttf","",20) ;
   textscore = game.loadText("Arial.ttf","",20) ;
   textenergy = game.loadText("Arial.ttf","",20) ;
   help.setColor(200,200,200) ;
   
   game.setGameTitle("Рарити лучшая пони") ;

   rar_x = 400;
   rar_y = 550 ;
   v0 = 200;
   vd = 300;
   dir = DIR_WAIT ;
   nextdiamond = MINPERIOD ;
   timeleft = 60 ;
   score = 0 ;
   gameover=false ;
   energy=0 ;
   venergy=33 ;
   jump_pause = 0 ;

   for (var i=0; i<DIAMOND_COUNT; i++)
     textcost[i]=game.loadText("Arial.ttf",diamond_cost[i],18) ;

   return true ;
}
 
function Render() {
   if (jump_pause<=0) rar.renderTo(rar_x,rar_y) ;

   for (var i=0; i<diamonds.length; i++)
      if (diamonds[i].y<700)
        spr[diamonds[i].spr_idx].renderTo(diamonds[i].x,diamonds[i].y) ;
   help.printTo(220,10) ;
   textleft.printTo(610,50) ;
   textscore.printTo(610,20) ;

   if (energy<JUMP_ENERGY) {
     textenergy.setColor(200,200,0) ;
     textenergy.setText("Подготовка\nтелепортации: "+Math.floor(energy)+"%") ;
   }
   else {
     textenergy.setColor(0,200,0) ;
     textenergy.setText("Телепортация\nготова!") ;
   }         
   textenergy.printTo(610,80) ;                     

   for (var i=0; i<DIAMOND_COUNT; i++) {
      spr[i].renderTo(20+50*i,30) ;
      textcost[i].printTo(15+50*i,70) ;
   }                                    

   return true ;
}

function Frame(dt) {

   if (jump_pause<=0) {
   if (game.isKeyDown(KEY_LEFT)) setDir(DIR_LEFT) ;
   if (game.isKeyDown(KEY_RIGHT)) setDir(DIR_RIGHT) ;	   
   if (game.isKeyDown(KEY_DOWN)) setDir(DIR_WAIT) ;	
   if (game.isKeyDown(KEY_SPACE)&&(energy>=JUMP_ENERGY)) {
      energy=0 ;
	  snd_jump.play() ;
	  jump_pause = 0.5 ;
      if (dir==DIR_LEFT) { 
        rar_x = rar_x - 300 ;	
        if (rar_x<BORDERSPACE) rar_x=BORDERSPACE ;
      }
      if (dir==DIR_RIGHT) {
        rar_x = rar_x + 300 ;	
        if (rar_x>800-BORDERSPACE) rar_x=800-BORDERSPACE ;
      }
   }	

   if ((rar_x<BORDERSPACE)&&(dir==DIR_LEFT)) setDir(DIR_WAIT) ;
   if ((rar_x>800-BORDERSPACE)&&(dir==DIR_RIGHT)) setDir(DIR_WAIT) ;

   if (dir==DIR_LEFT)
     rar_x = rar_x - v0*dt ;	
   if (dir==DIR_RIGHT)
     rar_x = rar_x + v0*dt ;	
   }

   for (var i=0; i<diamonds.length; i++) {
      if (diamonds[i].y<700)
        diamonds[i].y+=vd*dt ;
      if (isDiamondOverRar(diamonds[i])) {
        diamonds[i].y=1000 ;
        snd_eat.play() ;
        score+=diamond_cost[diamonds[i].spr_idx] ;
      }
   }

   nextdiamond-=dt ;
   if ((nextdiamond<0)&&(Math.floor(timeleft)>0)) {
     nextdiamond = MINPERIOD ;
     var diamond = { 
       spr_idx:getRandomInt(0,DIAMOND_COUNT-1),
       x:getRandomInt(100,800-100), y:-50
     }
     diamonds.push(diamond) ;
   }

   if (jump_pause>0) jump_pause-=dt ;

   if (Math.floor(timeleft)>0)  {
      timeleft-=dt ;
      textleft.setText("Осталось: "+Math.floor(timeleft)+" сек") ;
    }

   energy+=venergy*dt ;

   textscore.setText("Очки: "+score) ;

   return true ;
}

