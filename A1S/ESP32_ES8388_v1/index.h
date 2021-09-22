const char PAGE_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pl">
<head>
<meta charset="utf-8">
<title>@radio Teren</title>
<link rel="manifest" href="teren.web.json"> 
<meta name="viewport" content="width=device-width, user-scalable=no" />
<meta name="mobile-web-app-capable" content="yes">
<meta name="theme-color" content="#4564a4">
<link rel="shortcut icon" type="image/svg+xml" size="any" href="radio.svg"> 
<link rel="icon" type="image/svg+xml" href="radio.svg">  
<style>
body{background-color:#fafafa;color:#fff;padding:0;margin:0;overflow:hidden;background-image:url(radio.svg);background-blend-mode:color-dodge;background-repeat:no-repeat;background-position:center 45vh;background-size:50vh}
*,html{font-size:14pt;font-family:Arial,Helvetica,sans-serif;color:#222;text-shadow: 2px 2px 2px #888;}
.wrap {z-index:2;margin:auto;padding:0.5rem;max-width:630px; }
.col {width:100%;text-align:center;margin:.25em auto;}
.grid{display:grid;grid-gap:.25em; margin-top:0.2em;}
.col-sm{grid-template-columns:repeat(auto-fit,minmax(50px,1fr));}
.col-md{grid-template-columns:repeat(auto-fit,minmax(80px,1fr));}
.col-lg{grid-template-columns:repeat(auto-fit,minmax(90px,1fr));}
.blue button {background-color:#f5be47;}  
button{background:#df513b;color:white;border:none;box-shadow:1px 1px 1px 0px #666;border-radius:0.75em;padding: 0.5em 0.1em;margin: 0.2em 0;}
h3{margin:.3em 0}
h3,#info {text-shadow: none;}
#czas,#info{font-size:.8rem;font-family:monospace}
#info{min-height:5em}
.active,button:active,button:visited{background-color:#4564a4!important;color:white!important;}
img,svg{display:grid;align-content:center;justify-content:space-around;max-width:10em;margin:auto}
div#app span{width:3em;height:1.3em;background:#e7e3c1;padding:.3em;margin:auto;border-radius:.5em} 
.eq button{background:#e7e3c1;color:#0f2c4b;font-size:.8rem}
@media (orientation: landscape) {body {overflow:auto;}}
</style>
<script>
const _$=e=>document.querySelector(e);
const _$$=e=>document.querySelectorAll(e);
let url = "/";
document.addEventListener("DOMContentLoaded",()=>{
  let urlObj = new URL(window.location.href);
  url = urlObj.searchParams.get("url") || url;
  console.log(url);
  sn();
  setInterval(()=>{sn()},15000);
});

let sn=(p="radio?n=0")=>{
fetch(url+p)
.then(r => {return r.text()})
.then(t => {
  console.log(t);
  opisz(t); 
  setTimeout(()=>{opisz(t)},3000);   
}).catch(e => {console.log(e)})  
}

const btnActive=(lSTA)=>{
  _$$("div.st button").forEach((b)=>{if (b) b.classList.remove("active")});
  if(lSTA>=0 && _$("#st"+lSTA)) _$("#st"+lSTA).classList.add("active");
}
const opisz=(o)=>{
  let a = o.split("!");
  let lastSTA = parseInt(a[0]);
  btnActive(lastSTA);
  _$("#sta").textContent = lastSTA+1;
  _$("#vol").textContent = a[1];
  _$("#info").innerHTML  = "wifi:"+a[2]+"dB";
  if(a[3]) _$("#info").innerHTML += "<br />"+a[3];
  if(a[4]) _$("#info").innerHTML += "<br />"+a[4];
  if(a[5]) _$("#info").innerHTML += "<br />"+a[5];
  if(a[6]) _$("#info").innerHTML += "<br />"+a[6];
  if(a[7]) _$("#info").innerHTML += " "+a[7];
}
</script>
</head>
<body>

<div class="wrap">
  <div class="col">
    <h3>@radio Teren</h3>
    <div id="info"></div>
  </div>
  <div class="col" id="app">
      <div class="grid col col-lg blue">
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
  <div class="grid col col-md red st">
    <button onClick='sn("radio?t=0")' id="st0">TOK-FM</button>
    <button onClick='sn("radio?t=1")' id="st1">RNŚ</button>
    <button onClick='sn("radio?t=2")' id="st2">357</button>
    <button onClick='sn("radio?t=3")' id="st3">Sw-Jzz</button>
    <button onClick='sn("radio?t=4")' id="st4">Sw-Cla</button>
    <button onClick='sn("radio?t=5")' id="st5">Sw-Pop</button>
    <button onClick='sn("radio?t=6")' id="st6">Cl-FM</button>
    <button onClick='sn("radio?t=7")' id="st7">101-J</button>
  </div>
  <div class="grid col col-md blue">
    <button onClick='sn("radio?n=0")' class="btn ex">Info</button>
    <button onClick='sn("radio?start=0")' class="btn ex">Mute</button>
    <button onClick='sn("radio?j=0")' class="btn ex">Replay</button>
    <button onClick='sn("radio?z=0")' class="btn ex">Reset</button>
    <button onClick='window.location.reload(true);' class="btn ex">Reload</button>    
  </div>
  
<hr /> 

  <div class="grid col col-sm eq">
    <button onClick='sn("radio?q=0")' >Mowa</button>
    <button onClick='sn("radio?q=1")' >Muza</button>
    <button onClick='sn("radio?q=2")' >Jazz</button>
    <button onClick='sn("radio?q=3")' >Flat</button>
  </div>  
  <div class="grid col col-sm eq">
    <button onClick='sn("radio?i=0")' >Mute+</button>
    <button onClick='sn("radio?i=1")' >Mute-</button>
    <button onClick='sn("radio?i=2")' >Enable+</button>
    <button onClick='sn("radio?i=3")' >Enable-</button>
  </div>  

</div>
</body>
</html>
)=====";
