const char PAGE_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pl">
<head>
<meta charset="utf-8">
<title>@radio SALON</title>
<link rel="manifest" href="./teren.web.json">
<meta name="viewport" content="width=device-width, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no">
<meta name="HandheldFriendly" content="true">
<meta name="mobile-web-app-capable" content="yes"> 
<meta name="theme-color" content="#4564a4">
<link rel="shortcut icon" type="image/svg+xml" size="any" href="radio.svg"> 
<link rel="icon" type="image/svg+xml" href="radio.svg">  
<style>
/*body{background-color:#fafafa;color:#fff;}*/
*,html{color:#eee;}
body{background-color:#222;}
body{padding:0;margin:0;overflow:hidden;background-image:url(radio.svg);background-blend-mode:color-dodge;background-repeat:no-repeat;background-position:center 45vh;background-size:50vh}
*,html{font-size:14pt;font-family:Arial,Helvetica,sans-serif;text-shadow: 2px 2px 2px #888;}
.wrap {z-index:2;margin:auto;padding:0.5rem;max-width:600px; }
.grid{display:grid;grid-gap:.25em; margin-top:0.2em;}
.col {width:100%;text-align:center;margin:.25em auto;}
.col{grid-template-columns:repeat(auto-fit,minmax(60px,1fr));}
.col-sm{grid-template-columns:repeat(auto-fit,minmax(40px,1fr));}
.col-md{grid-template-columns:repeat(auto-fit,minmax(80px,1fr));}
.col-lg{grid-template-columns:repeat(auto-fit,minmax(90px,1fr));}
.prima button {background-color:#f5be47;}  
h3{margin:.3em 0}
h3,#info {text-shadow: none;}
#czas,#info{font-size:.8rem;font-family:monospace}
#info{min-height:5em}
button{background:#df513b;color:white;border:none;box-shadow:1px 1px 1px 0px #666;border-radius:0.75em;padding: 0.5em 0.1em;margin: 0.2em 0;}
.active,button:active,button:visited{background-color:#4564a4!important;color:white!important;}
.small button{font-size:.85rem}
.eq button{background:#e7e3c1;color:#0f2c4b}
.prima button{color:black}
img,svg{display:grid;align-content:center;justify-content:space-around;max-width:10em;margin:auto}
div#app span{width:3em;height:1.3em;background:#e7e3c1;padding:.3em;margin:auto;border-radius:.5em;color: #222;} 
input#slij,input#nazwa{color:black;text-shadow:none;font-size:0.85rem;padding: 0.2em 0;}
input#slij {width:97%;}
input#nazwa {width:76%;}
button#btn-slij {width:19%;background:#4169e1}
.stu button {background:#4169e1;}

div.gr {display:inline-block;margin-right:1em;}
div.gr button {
    font-size: 0.9rem;
    padding: 0.33em;
    background: #369;
    max-width: 88%;
    text-align: left;
    white-space: nowrap;
    overflow: hidden;
}


div.gr button:first-child{background:darkcyan;}

@media (orientation: landscape) {body {overflow:auto;}}
</style>
<script>
const _$=e=>document.querySelector(e);
const _$$=e=>document.querySelectorAll(e);
const radio="radio";
let nr=0;
let url = "/";

let lastSTA = 0;
let locale = {};
locale[radio] = {};

const ref=(t=3)=>{setTimeout(()=>{sn()},t*1000);}
const unID = () => [...new Array(8)].map(() => String.fromCharCode(97 + Math.random()*26)).join('')

document.addEventListener("DOMContentLoaded",()=>{
  let urlObj = new URL(window.location.href);
  url = urlObj.searchParams.get("url") || url;
  console.log(url);
  localStorage.setItem("url", url);
  ref(1);
  setInterval(()=>{sn()},15000);
  fetch(url+"stacje.json")
  .then(j => {return j.json()})
  .then(s => {
    stacje(s);
  }).catch(e => {console.log(e)})
  if (localStorage.getItem(radio)){
      locale[radio] = JSON.parse(localStorage.getItem(radio))[radio];
      opiszLocale();
  }
});

let opiszLocale=()=>{
_$("#plus").innerHTML ="";
      let i=0;
      for(let k in locale[radio]){
        wpisz(i++,locale[radio][k],k);
      }
}

let stacje=(st)=>{
  let h="";
  st.forEach((s,i)=>{
    if (s.n) {h+='<button id="sta'+i+'" data-n="'+i+'" data-s="'+s.s+'" data-a="'+s.a+'" onClick="sta(this)">'+s.n+'</button>';}
  });
  _$("#stacje").innerHTML=h;
}

let sn=(p="radio?n=0")=>{
fetch(url+p)
.then(r => {return r.text()})
.then(t => {
  opisz(t); 
  setTimeout(()=>{opisz(t)},3000);   
}).catch(e => {console.log(e)})  
}

const btnActive=(btnNR,cl="st")=>{
  _$$("div."+cl+" button").forEach((b)=>{if (b) b.classList.remove("active")});
  if(btnNR>=0 && _$("#st"+btnNR)) _$("#"+cl+btnNR).classList.add("active");
}
const opisz=(o)=>{
  let a = o.split("!");
  
  let lastSTA = parseInt(a[0]);
  //let lastEQ  = a.pop();
  btnActive(lastSTA);
  //btnActive(lastEQ,"eq");
  _$("#sta").textContent = lastSTA+1;
  _$("#vol").textContent = a[1];
  _$("#info").innerHTML  = "wifi:"+a[2]+"dB";
  if(a[3]) _$("#info").innerHTML += "<br />"+a[3];
  if(a[4]) _$("#info").innerHTML += "<br />"+a[4];
  if(a[5]) _$("#info").innerHTML += "<br />"+a[5];
  if(a[6]) _$("#info").innerHTML += "<br />"+a[6];
  if(a[7]) _$("#info").innerHTML += " "+a[7];
}

let sta=(ten)=>{
  let lastStream = ten.dataset.s;
  _$("#slij").value=lastStream;
  _$("#nazwa").value=ten.textContent;
  localStorage.setItem("lastStream", lastStream);
  sn("radio?x="+lastStream);
  nr = ten.dataset.n;
  //sn("radio?y="+ten.dataset.a);
  sn("radio?z="+nr);
  ref();
  ref(5);
}

let wpisz=(nr,x,nazwa)=>{
    _$("#plus").innerHTML += '<div class="gr"><button onClick="del(this)">&nbsp;&#x2715&nbsp;</button>&nbsp;<button data-n="'+nr+'" data-s="'+x+'" data-a="0" onclick="sta(this)">'+nazwa+'</button></div>';
}

let slij=()=>{
  let x=_$("#slij").value;
  if (x) {
    sn("radio?x="+x);
    //let xx = x.replace(/^http[s]?:\/\/(www\.)?(.*)?\/?(.)*/,"");
    let xx = x.replace(/^http[s]?:\/\/(www\.)?/,"");
    let nazwa = _$("#nazwa").value ? _$("#nazwa").value : xx;
    ZapiszLocale(radio,nazwa,x);
    wpisz(nr,x,nazwa);
    nr++;
    ref();
    ref(5);
  }
}
let del=(t)=>{
  let d = t.nextElementSibling.textContent;
  delete locale[radio][d]
  opiszLocale();
  localStorage.setItem(radio, JSON.stringify(locale));
}
let ZapiszLocale=(root,key,val)=>{
    if(root) locale[root][key]=val;
    localStorage.setItem(radio, JSON.stringify(locale));  
}
</script>
</head>
<body>

<div class="wrap">
  <div class="col">
    <h3>@radio Salon</h3>
    <div id="info"></div>
  </div>
  <div class="col" id="app">
      <div class="grid col col-lg prima">
        <button onClick='sn("radio?v=m")'>Vol-</button>
        <span id="vol"></span>
        <button onClick='sn("radio?v=p")'>Vol+</button>
      </div>
      <div class="grid col col-lg red">
        <button onClick='sn("radio?s=m")'>Sta-</button>
        <span id="sta"></span>
        <button onClick='sn("radio?s=p")'>Sta+</button>
      </div>   
  </div>
  <div class="grid col col-md st">
    <button onClick='sn("radio?s=0")' id="st0">TOK-FM</button>
    <button onClick='sn("radio?s=1")' id="st1">RN??</button>
    <button onClick='sn("radio?s=2")' id="st2">357</button>
    <button onClick='sn("radio?s=3")' id="st3">Sw-Jzz</button>
    <button onClick='sn("radio?s=4")' id="st4">Sw-Cla</button>
    <button onClick='sn("radio?s=5")' id="st5">RMF-Cl</button>
    <button onClick='sn("radio?s=6")' id="st6">Cl-FM</button>
    <button onClick='sn("radio?s=7")' id="st7">PR2</button>
    
    <button onClick='sn("radio?s=8")'  id="st8">Niezap</button>
    <button onClick='sn("radio?s=9")'  id="st9">RockS</button>
    <button onClick='sn("radio?s=10")' id="st10">101-Jz</button>
    <button onClick='sn("radio?s=11")' id="st11">Szczec</button>
  </div>

  <div>
    <input id="slij" placeholder="Stream URL"  />
    <input id="nazwa" placeholder="Nazwa" /> <button onClick="slij()" id="btn-slij">??lij</button><br />
  </div>
  <div class="" id="plus"></div>
  <table id="local"></table>
  
  <div class="grid col col-md st stu" id="stacje"></div>
  
  <div class="grid col prima small">
    <button onClick='sn("radio?n=0")' class="btn ex">Info</button>
    <button onClick='sn("radio?start=0")' class="btn ex">Mute</button>   
    <button onClick='window.location.reload(true);' class="btn ex">Reload</button>    
    <button onClick='sn("radio?v=z")' class="btn ex">Zapisz</button>    
  </div>
  
    <div class="grid col prima small">
      <button class="btn3" onclick='st("-9")'>Reboot</button>
      <button class="btn3" onclick='st("-1")'>Music</button>
      <button onClick='sn("radio?r=0")' class="btn ex">Replay</button>
      <button class="btn3" onclick='st("-2")'>Sleep</button>
    </div>

<hr /> 

  <div class="grid col small eq">
    <button onClick='sn("radio?q=0");' id="eq0">Mowa</button>
    <button onClick='sn("radio?q=1");' id="eq1">Muza</button>
    <button onClick='sn("radio?q=2");' id="eq2">Jazz</button>
    <button onClick='sn("radio?q=3");' id="eq3">TXT</button>
    <button onClick='sn("radio?q=4");' id="eq4">Flat</button>
  </div>

</div>
</body>
</html>

)=====";
