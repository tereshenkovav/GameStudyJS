// Позиция лодки по x
var x ;
// Текущая скорость лодки
var v ;
// Позиция крюка по y
var line_y ;
// Признак выпущенного крюка
var isline ;

// Спрайт лодки
var boat ;
// Линия лески
var line ;
// Прямоугольник моря
var sea ;
// Спрайт крюка
var hook ;
// Спрайт краба
var crab ;

// Остаток лесок
var rod_left ;
// Очки
var score ;

// Текстовый блок очков
var text_score ; 
// Текстовый блок лесок
var text_rod ; 

// Массивы рыб и их скоростей
var fishes = new Array() ;
var fishes_v = new Array() ;

// Массивы крабов и их скоростей
var crabs = new Array() ;
var crabs_v = new Array() ;

// Счетчик до следующей рыбы
var nextfish ;

// Модуль скорости лодки
const V0 = 150 ;

// Функция генерирует случайное число в заданном диапазоне
function getRandom(min, max){
  return Math.floor(Math.random() * (max - min + 1)) + min;
}


// Первая функция движка - инициализация игры
function Init(arg) {
// Позиция лодки
   x = 400; 
   v = 0 ;
// Остаток лесок
   rod_left = 10 ;
   score = 0 ;
// Время до новой рыбы
   nextfish = 2.0 ;
// По умолчанию, леска не выпущена
   isline = false ;

// Загрузка ресурсов и создание объектов
   boat = game.loadSprite('boat.png') ;
   hook = game.loadSprite('hook.png') ;
   crab = game.loadSprite('crab.png') ;
   line = game.createLine(0,0,0) ;
   sea = game.createRect(100,255,220) ;

// Создание текстовых блоков
   text_score = game.loadText("Arial.ttf","Очки: "+score,24) ;
   text_rod = game.loadText("Arial.ttf","Удочек: "+rod_left,24) ;

// Установка заголовка и фона игры
   game.setGameTitle("Рыбалка") ;
   game.setBackgroundColor(50, 200, 255) ;
   
// Запуск 25 крабов в случайном месте и скорости
   for (var i=0; i<25; i++) {
     crabs.push(getRandom(100,700)) ;
     crabs_v.push(getRandom(0,1)==0?getRandom(100,200):-getRandom(100,200)) ;
   }

   return true ;
}
 
// Вторая функция движка - вывод графики
function Render() {
// Рисуем море
   sea.drawTo(0,140,800,600) ;
// Рисуем лодку
   boat.renderTo(x,100) ;
// Рисуем текстовые блоки
   text_score.printTo(20,5) ;
   text_rod.printTo(140,5) ;
     
// Вывод всех рыб
   for (var i=0; i<fishes.length; i++)
     fishes[i].render() ;

// Если выпущена леска, то рисуем линию и крюк
   if (isline) {
     line.drawTo(x,150,x,line_y-10) ;
     hook.renderTo(x,line_y) ;
   }

// Рисуем всех крабов
   for (var i=0; i<crabs.length; i++) 
     crab.renderTo(crabs[i],570) ;

   return true ;
}

// Третья функция движка - обновление состояния игры за интервал времени dt
function Frame(dt) {
// Пересчет позиции лодки
   x = x + v*dt ;	

// Если лодка дошла до края, то стоп   
   if ((x>700)&&(v>0)) v = 0 ;
   
   if ((x<100)&&(v<0)) v = 0;

// Если выпущен крюк, то обновляем позицию крюка
   if (isline) {
     line_y = line_y+ 150*dt ;
// Если крюк дошел до крабов, то отключаем крюк, убираем леску и выводим текст
     if (line_y>=570) {
       isline=false ;
       rod_left-- ;
       text_rod.setText("Удочек: "+rod_left) ;
     }
   }

// Уменьшение счетчика до новой рыбы                 
   nextfish=nextfish-dt ;
// Если время запускать рыбу
   if (nextfish<=0) {
// Ставим счетчик заново
     nextfish = 2.0 ;

// Случайно выбираем направление рыбы - слева или справа
     var isleft = getRandom(0,1)==0 ;
// Случайная скорость рыбы 
     var speed = getRandom(100,200) ;

// Грузим случайный спрайт рыбы из трех
     var newfish = game.loadSprite("fish"+getRandom(1,3)+".png") ;
// Ставим рыбу в случайную позицию по Y, и слева/справа, как выбрали ранее
     newfish.setXY(isleft?-100:900,getRandom(200,500)) ;
// Отражаем по горизонтали спрайт рыбы, если нужно
     newfish.mirrorHorz(isleft) ;
// Добавляем скорость рыбы со знаком (слева или справа плывет)
     fishes_v.push(isleft?speed:-speed) ;
// И саму рыбу в массив
     fishes.push(newfish) ;
   }

// Обход массива рыбы не через цикл FOR, а через индекс
   var i=0;
   while (i<fishes.length) {
// Двигаем спрайт каждой рыбы по его скорости
     fishes[i].setX(fishes[i].getX()+fishes_v[i]*dt) ;
// Если рыба совпала с крючком и крюк выпущен
     if (fishes[i].isContactWith(hook)&&isline) {
// Удалем рыбу и её скорость из массива
       fishes.splice(i,1) ;		 
       fishes_v.splice(i,1) ;	
// Отключаем показ крюка	 
       isline=false ;
// Добавляем очки и выводим текст
       score++ ;
       text_score.setText("Очков: "+score) ;
     }
// В противном случае, увеличиваем индекс для обхода следующего элемента
     else 
       i++ ;
   }

// Обновляем позиции всех крабов по скорости, отражаем их движение от краев
   for (var i=0; i<crabs.length; i++) {
     crabs[i]=crabs[i]+crabs_v[i]*dt ;
     if ((crabs[i]<50)&&(crabs_v[i]<0)) crabs_v[i]=-crabs_v[i] ;
     if ((crabs[i]>750)&&(crabs_v[i]>0)) crabs_v[i]=-crabs_v[i] ;
   }

// Если нажата кнопка лево и крюк не выпущен, то запускаем лодку влево
   if (game.isKeyDown(KEY_LEFT)&&(!isline)) {
     v = -V0 ; boat.mirrorHorz(false) ;
   }
// Если нажата кнопка вправо и крюк не выпущен, то запускаем лодку вправо
   if (game.isKeyDown(KEY_RIGHT)&&(!isline)) {
     v = V0 ; boat.mirrorHorz(true) ;
   }
// Если нажато вниз или вверх, то стоп лодке
   if (game.isKeyDown(KEY_UP)||game.isKeyDown(KEY_DOWN)) v = 0 ; 

// Если нажат пробел и крюк не выпущен
   if (game.isKeyDown(KEY_SPACE)&&(!isline)) {
// Стоп лодке
     v = 0 ;
// Выпускаем крюк и ставим его начальное положение 
     isline=true ;
     line_y=170 ;
   }

   return true ;
}
