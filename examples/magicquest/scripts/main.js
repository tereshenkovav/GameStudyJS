var trixie_walk ;
var trixie_wait ;
var horn ;
var marker ;
var zone ;
var magic ;

var text ;
var text_ico ;

var sound_galop ;
var sound_magic ;

// Scene data
var back1 ;
var back2 ;
var back3 ;
var back4 ;
var back1_1 ;
var back2_1 ;
var back2_2 ;
var back3_1 ;
var stone ;
var stone_ico ;
var baloons ;
var baloons_ico ;
var baloon ;
var fog ;
var fog_ico ;
var cloud ;
var cloud_ico ;
var modpie ;
var modpie_ico ;

var scenen = -1 ; // Нужно значение для старта

// Scene data
var intromsg2_show = false ;
var intromsg3_show = false ;
var stone_removed = false ;
var cloud_removed = false ;
var baloon_effect = false ;
var baloon_pos = {x:0, y: 0} ;
var stone_effect = false ;
var stone_pos = {x:0, y: 0} ;
var cloud_effect = false ;
var cloud_transp = 0 ;

var way = new Array() ;
var markerpos = [
           {x:160,y:500+23},
           {x:137,y:500+30},
           {x:110,y:500+25},
           {x:95,y:500+40},
           {x:115,y:500+47}
          ] ;

var spell_liftup = [ 0,2,4,0 ] ;
var spell_fog = [ 1,2,1,3 ] ;

var objs = new Array() ;

var tekspell = new Array() ;

var vel = {x:0,y:0,z:0} ;
var isgo = false ;
var ismirr = false ;
var line ;
var msg = "" ;

var tekpos = {x:0, y:0, z:0} ;
var targetidx ;
var selobjectidx=-1 ;
var targetedobjectidx=-1 ;

var waystack = new Array() ;
var markerstack = new Array() ;

const BASEV = 100 ;
const ZFRONT = 1 ;
const ZBACK = -1;

function sign(n) {
  if (n<0) return -1 ; else return 1 ;
}

function dist2(x1,x2,y1,y2) {
  return (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2) ;
}

function isReachTarget() {
  return dist2(tekpos.x,way[targetidx].x,tekpos.y,way[targetidx].y)<5*5 ;
}

function goTargetIdx(idx) {
  tekspell = new Array() ; // Очистка магии при уходе

  sound_galop.play()
  isgo=true ;
  targetidx=idx ;
  var alldist = Math.sqrt(dist2(way[targetidx].x,tekpos.x,way[targetidx].y,tekpos.y)) ;
  var t = alldist/BASEV ;
  if (t<=0) t=0.1 ;
  t*=(1+way[targetidx].z)*(1+way[targetidx].z) ;
  vel.x=(way[targetidx].x-tekpos.x)/t ;
  vel.y=(way[targetidx].y-tekpos.y)/t ; 
  vel.z=(way[targetidx].z-tekpos.z)/t ; 
}

function trySetTarget(x,y) {
  var mindistfirst = 99999 ;
  var mindistlast = 99999 ;
  var idxfirst=-1 ;  
  var idxlast=-1 ;  
  for (var i=0; i<way.length; i++) {
    if (dist2(way[i].x,tekpos.x,way[i].y,tekpos.y)<mindistfirst) {
      mindistfirst=dist2(way[i].x,tekpos.x,way[i].y,tekpos.y) ;
      idxfirst=i ;
    }
    if (dist2(way[i].x,x,way[i].y,y)<mindistlast) {
      mindistlast=dist2(way[i].x,x,way[i].y,y) ;
      idxlast=i ;
    }
  }
  
  if ((idxfirst!=-1)&&(idxlast!=-1)) {    
    waystack = new Array() ;
    if (idxfirst!=idxlast) {
      var delta = sign(idxlast-idxfirst) ;
      for (var i=idxfirst+delta; i!=idxlast; i+=delta) 
        waystack.push(i) ;
      waystack.push(idxlast) ;      
    }
    goTargetIdx(idxfirst) ;
  }   
}

function getOverObjectIdx() {
   var mp = game.getMousePos() ;
   for (var i=0; i<objs.length; i++) {
     objs[i].spr.setXY(objs[i].x,objs[i].y);
     if (objs[i].spr.isPointIn(mp.x,mp.y)) return i ;
   }
   return -1 ;
}

function drawObjIco(idx) {
  objs[idx].spr_ico.renderTo(740,530) ;
}

function drawObjIcoAndText(idx) {
  drawObjIco(idx) ;
  text_ico.setText(objs[idx].caption) ;
  text_ico.printTo(710,570) ;
}

function setMsg(str,len) {
  msg=str ;
  system.setTimeout("msg=''",len) ; 
}

function playMarkerStack() {
  if ((!marker.isPlayed())&&(markerstack.length>0)) {
     marker.setXY(markerpos[markerstack[0]].x,markerpos[markerstack[0]].y) ;
     marker.playOneTime() ;
     markerstack.shift() ;
     sound_magic.play() ;
  }
}

function isTekSpellEqual(spell) {
   // Must fit any length!!!
   for (var i=0; i<spell.length; i++) {
      if (spell[i]!=tekspell[i]) return false;
   }
   return true ;
}

function isTekSpellInverseEqual(spell) {
   // Must fit any length!!!
   for (var i=0; i<spell.length; i++) {
      if (spell[spell.length-1-i]!=tekspell[i]) return false;
   }
   return true ;
}

function loadScene1() {
   var oldscenen = scenen ;
   scenen=1 ;

   // Сначала путь, чтобы добавили потом
   way = [
        {x:331,y:465,z:0.0},
        {x:321,y:431,z:0.1},
        {x:263,y:386,z:0.2},
        {x:291,y:336,z:0.2},
        {x:368,y:340,z:0.2},
        {x:368,y:237,z:0.2},
      ] ;

   objs = new Array() ;
   // Тупое дублирование кода, нужно сохранять состояние сцены, а не создавать заново
   if (!stone_removed)
     objs.push( { x:438,y:240,spr:stone,spr_ico:stone_ico,caption:"Камень", way_i:5, z:ZBACK } ) ;
   else {
     way.push({x:408,y:263,z:0.2}) ;
     way.push({x:471,y:261,z:0.2}) ;
     way.push({x:535,y:260,z:0.2}) ;
   }
   if (!cloud_removed)
     objs.push( { x:538,y:135,spr:cloud,spr_ico:cloud_ico,caption:"Облако", way_i:8, z:ZBACK } ) ;
   else {
     way.push({x:573,y:205,z:0.3}) ;
     way.push({x:541,y:171,z:0.4}) ;
   }

   if (oldscenen==3) {
     tekpos.x = way[0].x ;
     tekpos.y = way[0].y ;
     tekpos.z = way[0].z ;
     trySetTarget(way[1].x,way[1].y) ;
   }
   if (oldscenen==4) {
     tekpos.x = way[10].x ;
     tekpos.y = way[10].y ;
     tekpos.z = way[10].z ;
     trySetTarget(way[9].x,way[9].y) ;
   }
}

function loadScene2() {
   scenen=2 ;

   objs = new Array() ;
   objs.push( { x:438,y:360,spr:baloons,spr_ico:baloons_ico,caption:"Шарики", way_i:5, z:ZFRONT } ) ;

   way = [
        {x:628,y:344,z:0.6},
        {x:686,y:346,z:0.3},
        {x:755,y:365,z:0.2},
        {x:672,y:377,z:0.1},
        {x:611,y:386,z:0.1},
        {x:509,y:397,z:0.0},
        {x:284,y:409,z:0.0},
        {x:109,y:412,z:0.0},
        {x:22,y:413,z:0.0}
      ] ;

   tekpos.x = way[8].x-100 ;
   tekpos.y = way[8].y ;
   tekpos.z = way[8].z ;
   ismirr = true ;
   trySetTarget(way[7].x,way[7].y) ;
}

function loadScene3() {
   var oldscenen = scenen ;
   scenen=3 ;

   objs = new Array() ;
   objs.push( { x:561+75,y:287+40,spr:fog,spr_ico:fog_ico,caption:"Туман", way_i:3, z: ZBACK } ) ;

   way = [
        {x:775,y:420,z:0.0},
        {x:626,y:420,z:0.0},
        {x:545,y:415,z:0.1},
        {x:462,y:375,z:0.1},
        {x:386,y:385,z:0.2},
        {x:324,y:350,z:0.3},
        {x:255,y:323,z:0.4},
        {x:279,y:277,z:0.5},
        {x:298,y:282,z:0.6},
        {x:327,y:265,z:0.7}
      ] ;

   if (oldscenen==-1) {
     tekpos.x = way[4].x ;
     tekpos.y = way[4].y ;
     tekpos.z = way[4].z ;
     ismirr = false ;
   }
   if (oldscenen==1) {
     tekpos.x = way[9].x ;
     tekpos.y = way[9].y ;
     tekpos.z = way[9].z ;
     trySetTarget(way[8].x,way[8].y) ;
   }
   if (oldscenen==2) {
     tekpos.x = way[0].x+100 ;
     tekpos.y = way[0].y ;
     tekpos.z = way[0].z ;
     trySetTarget(way[1].x,way[1].y) ;
   }

}

function loadScene4() {
   scenen=4 ;

   objs = new Array() ;
   objs.push( { x:565,y:364,spr:modpie,spr_ico:modpie_ico,caption:"Мод Пай", way_i:2, z: ZBACK } ) ;

   way = [
        {x:165,y:287,z:0.2},
        {x:256,y:358,z:0.1},
        {x:415,y:414,z:0.0}
      ] ;

   tekpos.x = way[0].x ;
   tekpos.y = way[0].y ;
   tekpos.z = way[0].z ;
   trySetTarget(way[1].x,way[1].y) ;

}

function Init() {
   back1 = game.loadSprite("back1.jpg") ;   
   back1_1 = game.loadSprite("back1_1.png") ;
   back1.setHotSpot(0,0) ;
   back1_1.setHotSpot(0,0) ;   

   back2 = game.loadSprite("back2.jpg") ;
   back2.setHotSpot(0,0) ;
   back2_1 = game.loadSprite("back2_1.png") ;
   back2_2 = game.loadSprite("back2_2.png") ;
   back2_1.setHotSpot(0,0) ;
   back2_2.setHotSpot(0,0) ;

   back3 = game.loadSprite("back3.jpg") ;
   back3.setHotSpot(0,0) ;
   back3_1 = game.loadSprite("back3_1.png") ;
   back3_1.setHotSpot(0,0) ;

   back4 = game.loadSprite("back4.jpg") ;
   back4.setHotSpot(0,0) ;

   sound_galop = game.loadSound("galop.ogg") ;
   sound_galop.setLoop(true) ;
   sound_magic = game.loadSound("magic.ogg") ;

   horn= game.loadSprite("horn.png") ;
   horn.setHotSpot(0,0) ;
   marker = game.loadAnimation("marker.png",30,34,16,16) ;
   zone = game.loadSprite("zone.png") ;

   magic= game.loadAnimation("magic.png",100,100,18,6) ;
   magic.play() ;

   stone_ico = game.loadSprite("stone_ico.png") ;
   stone = game.loadSprite("stone.png") ;
   baloons_ico = game.loadSprite("baloons.png") ;
   baloons = game.loadSprite("baloons.png") ;
   baloon = game.loadSprite("baloon.png") ;
   fog_ico = game.loadSprite("fog_ico.png") ;
   fog = game.loadSprite("fog.png") ;
   cloud_ico = game.loadSprite("cloud_ico.png") ;
   cloud = game.loadSprite("cloud.png") ;
   modpie_ico = game.loadSprite("modpie_ico.png") ;
   modpie = game.loadSprite("modpie.png") ;

   trixie_walk = game.loadAnimation("trixie_walk.png",94,90,16,16) ;
   trixie_walk.setHotSpot(47,90) ;
   trixie_walk.play() ;
   trixie_wait = game.loadAnimation("trixie_wait.png",92,82,6,6) ;
   trixie_wait.setHotSpot(46,82) ;
   //trixie_wait.play() ;
   system.setInterval("trixie_wait.playOneTime()",5000) ;

   text = game.loadText("Arial.ttf","",24) ;
   text.setColor(0x6f,0xaf,0xe2) ;

   text_ico = game.loadText("Arial.ttf","",18) ;
   text_ico.setColor(0xE0,0xE0,0xE0) ;

   game.setGameTitle("Хроники Трикси: техническая демо-версия") ;

   line = game.createLine(0,0,0) ;

   loadScene3() ;

   return true ;
}
 
function Render() {
   // Scene switch
   if (scenen==1) back1.renderTo(0,0) ;
   if (scenen==2) back2.renderTo(0,0) ;
   if (scenen==3) back3.renderTo(0,0) ;
   if (scenen==4) back4.renderTo(0,0) ;

   for (var i=0; i<objs.length; i++)
     if (objs[i].z==ZBACK)
       objs[i].spr.renderTo(objs[i].x,objs[i].y) ;

   if (isgo) {
     trixie_walk.mirrorHorz(ismirr) ;
     trixie_walk.setScale(100*(1-tekpos.z)) ;
     trixie_walk.renderTo(tekpos.x,tekpos.y) ;
   }
   else {
     trixie_wait.mirrorHorz(ismirr) ;
     trixie_wait.setScale(100*(1-tekpos.z)) ;
     trixie_wait.renderTo(tekpos.x,tekpos.y) ;
   }

   // Scene switch
   if (scenen==1) {
    if (!( ((targetidx==5)&&(vel.x<0))||(targetidx>=6)||(targetidx<=2)))
      back1_1.renderTo(307,227) ;
   }
   if (scenen==2) {
     back2_1.renderTo(303,356) ;
     back2_2.renderTo(524,372) ;
   }
   if (scenen==3) {
     back3_1.renderTo(402,361) ;
   }

   for (var i=0; i<objs.length; i++)
      if (objs[i].z==ZFRONT)
        objs[i].spr.renderTo(objs[i].x,objs[i].y) ;

   horn.renderTo(0,500) ;
   for (var i=0; i<markerpos.length; i++)
      zone.renderTo(markerpos[i].x,markerpos[i].y) ;
   if (marker.isPlayed()) marker.render() ;

   var overobjectidx = getOverObjectIdx() ;

   if ((overobjectidx!=-1)&&(selobjectidx!=-1)) {
      if (overobjectidx==selobjectidx) 
         drawObjIcoAndText(selobjectidx) ;
      else 
         drawObjIco(overobjectidx) ;
   }
   else
   if (overobjectidx!=-1) 
     drawObjIco(overobjectidx) ;   
   else
   if (selobjectidx!=-1) 
     drawObjIcoAndText(selobjectidx) ;
  
   if (msg!="") {
     text.setText(msg) ;
     text.printTo(220,500)
   }

   if ((tekspell.length>0)&&(selobjectidx!=-1)) {
     var scalex = 100*objs[selobjectidx].spr.getWidth()/79 ;
     var scaley = 100*objs[selobjectidx].spr.getHeight()/79 ;
     magic.setScaleX(scalex) ;
     magic.setScaleY(scaley) ;
     magic.renderTo(objs[selobjectidx].x,objs[selobjectidx].y) ;
   }

  // for (var i=0; i<way.length-1; i++)
  //    line.drawTo(way[i].x,way[i].y,way[i+1].x,way[i+1].y) ;

   // Scene data
   if (baloon_effect) 
     baloon.renderTo(baloon_pos.x+10*Math.sin(2*game.getTotalTime()),baloon_pos.y) ;
   // Scene data
   if (stone_effect)
       stone.renderTo(stone_pos.x+10*Math.sin(2*game.getTotalTime()),stone_pos.y) ;
   // Scene data
   if (cloud_effect) {
       cloud.setAlpha(cloud_transp) ;
       cloud.render() ;
   }

   return true ;
}

function Frame(dt) {

   var mp = game.getMousePos() ;

   if (isgo) {
     tekpos.x += vel.x*dt ;	
     tekpos.y += vel.y*dt ;
     tekpos.z += vel.z*dt ;
     ismirr = (vel.x<0) ;
     if (isReachTarget()) {
       if (waystack.length==0) {
         sound_galop.stop() ;
         isgo=false ;
         if (targetedobjectidx!=-1)
            ismirr = objs[targetedobjectidx].x<tekpos.x ;
         // Scene data
         if (scenen==1) {
           if (targetidx==0) { loadScene3() ; return true ; }
           if (targetidx==10) { loadScene4() ; return true ; }
         }
         if (scenen==3) {
           if (targetidx==0) { loadScene2() ; return true ; }
           if (targetidx==9) { loadScene1() ; return true ; }
         }
         if (scenen==4) {
           if (targetidx==0) { loadScene1() ; return true ; }
         }
         if (scenen==2) {
           if (targetidx==0) setMsg("Похоже, эта тропинка никуда не ведёт.",3000) ;
           if (targetidx==8) { loadScene3() ; return true ; }
         }
       } else {
         goTargetIdx(waystack[0]) ;
         waystack.shift() ;
       }
     }
   }

   var overobjectidx = getOverObjectIdx() ;

   if (overobjectidx!=-1) {
     var idx = objs[overobjectidx].way_i ;
     if (idx<way.length) // Поправка на недоступные ячейки
       if (dist2(way[idx].x,tekpos.x,way[idx].y,tekpos.y)<5*5)
         selobjectidx=overobjectidx ;
   }
   // Дублирование с overobjectidx
   if (targetedobjectidx!=-1) {
     var idx = objs[targetedobjectidx].way_i ;
     if (idx<way.length) // Поправка на недоступные ячейки
       if (dist2(way[idx].x,tekpos.x,way[idx].y,tekpos.y)<5*5)
         selobjectidx=targetedobjectidx ;
   }
   if (selobjectidx!=-1) {
     var idx = objs[selobjectidx].way_i ;     
     if (dist2(way[idx].x,tekpos.x,way[idx].y,tekpos.y)>5*5)
       selobjectidx=-1 ;
   }

   if (game.isLeftButtonClicked()) {
     targetedobjectidx=-1 ;
     if (overobjectidx!=-1) {
        var idx = objs[overobjectidx].way_i ;
        if (idx<way.length) { // Поправка на недоступные ячейки
          trySetTarget(way[idx].x,way[idx].y) ;
          targetedobjectidx=overobjectidx ;
        }
     } 
     else {
        if (mp.y<480) trySetTarget(mp.x,mp.y) ;
     }
    
     if (selobjectidx!=-1) {
        
        if (objs[selobjectidx].spr_ico.isPointIn(mp.x,mp.y)) {
           // Scene data
	   if (objs[selobjectidx].caption=="Камень")
	      setMsg("Я не могу обойти этот камень",3000) ;
           if (objs[selobjectidx].caption=="Облако")
              setMsg("Из-за этого облака,\nя не могу подняться выше",4000) ;
           if (objs[selobjectidx].caption=="Мод Пай")
              setMsg("А вот и Мод Пай.\nМой поход успешно завершен.\nСпасибо за тест игры!",6000) ;
	   if (objs[selobjectidx].caption=="Шарики") {
	      if ((!baloon_effect)&&(!marker.isPlayed())) {
                setMsg("Упс!",2000) ;
                baloon_pos.x = objs[selobjectidx].x ;
                baloon_pos.y = objs[selobjectidx].y ;
                baloon_effect = true ;
                markerstack = spell_liftup.slice() ;
	      }
	   }
           if (objs[selobjectidx].caption=="Туман") {
              if (!marker.isPlayed()) {
              setMsg("Очень густой туман",3000) ;
              markerstack = spell_fog.slice() ;
              }
           }
	}        

        for (var i=0; i<markerpos.length; i++) 
           if (dist2(markerpos[i].x,mp.x,markerpos[i].y,mp.y)<15*15) 
              if (!marker.isPlayed()) {
                marker.setXY(markerpos[i].x,markerpos[i].y) ;
                marker.playOneTime() ;              
                sound_magic.play() ;
                tekspell.push(i) ;
              }

     }
     
   }
   
   playMarkerStack() ;

   if ((tekspell.length==4)&&(!marker.isPlayed())) {

      // Scene data
      if (isTekSpellEqual(spell_liftup)) {
         if (objs[selobjectidx].caption=="Камень") {
           stone_pos.x = objs[selobjectidx].x ;
           stone_pos.y = objs[selobjectidx].y ;
           stone_effect=true ;
           objs.splice(selobjectidx,1) ;
           selobjectidx=-1 ;
           stone_removed = true ;
         }
         else
           setMsg("Заклинание не действует",2000) ;
      }
      else
      if (isTekSpellEqual(spell_fog)) {
         if (objs[selobjectidx].caption=="Облако") {
           setMsg("Облако и так достаточно плотное",2000) ;
         }
         else
         if (objs[selobjectidx].caption=="Туман") {
           setMsg("Туман уже более чем густой",2000) ;
         }
         else
           setMsg("Заклинание не действует",2000) ;
      }
      else
      if (isTekSpellInverseEqual(spell_fog)) {
         if (objs[selobjectidx].caption=="Облако") {
           cloud_transp = 255 ;
           cloud_effect=true ;
           objs.splice(selobjectidx,1) ;
           selobjectidx=-1 ;
           cloud_removed = true ;
         }
         else
         if (objs[selobjectidx].caption=="Туман") {
           setMsg("Я делаю правильно, но моих сил\nне хватит на этот обширный туман",4000) ;
         }
         else
           setMsg("Заклинание не действует",2000) ;
      }
      else
         setMsg("Это какое-то неправильное заклинание",3000) ;
      tekspell = new Array() ;
   }

   // Scene data
   if (baloon_effect) {
     baloon_pos.y-=150*dt ;     
     if (baloon_pos.y<0) baloon_effect=false ;
   }
   // Scene data
   if (stone_effect) {
     stone_pos.y-=150*dt ;
     if (stone_pos.y<0) {
        stone_effect=false ;
        way.push({x:408,y:263,z:0.2}) ;
        way.push({x:471,y:261,z:0.2}) ;
        way.push({x:535,y:260,z:0.2}) ;
        setMsg("Он улетел. Путь свободен.",2000) ;
     }
   }
   // Scene data
   if (cloud_effect) {
     cloud_transp-=100*dt ;
     if (cloud_transp<0) {
        cloud_effect=false ;
        way.push({x:573,y:205,z:0.3}) ;
        way.push({x:541,y:171,z:0.4}) ;
        setMsg("А вот и вход в пещеру",2000) ;
      }
   }

   // Scene data
   if ((!intromsg2_show)&&(scenen==2)&&(!isgo)) {
     setMsg("Кто-то забыл воздушные шарики на поляне",3000) ;
     intromsg2_show = true ;
   }
   if ((!intromsg3_show)&&(scenen==3)&&(!isgo)) {
     setMsg("Мне нужно добраться до пещеры в горах.\nМод Пай пригласила меня поискать камни.",6000) ;
     intromsg3_show = true ;
   }

   return true ;
}

