const char PAGE_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pl">
<head>
<meta charset="utf-8">
<title>@radio RNS</title>
<link rel="manifest" href="teren.web.json"> 
<meta name="viewport" content="width=device-width,initial-scale=1">
<meta name="mobile-web-app-capable" content="yes">
<meta name="theme-color" content="#4564a4">
<link rel="shortcut icon" type="image/svg+xml" size="any" href="radio.svg"> 
<link rel="icon" type="image/svg+xml" href="radio.svg">  
<style>
*,html{font:normal 14pt verdana;color:#222}body{background-color:#fafafa;padding:.25em;color:#fff}.col{max-width:630px;margin:auto;text-align:center}h3{margin:.3em 0}button{border:none}#czas,#info{font-size:.8rem;font-family:monospace}#info{min-height:5em}.btn{background-color:#f5be47;padding:.65em .15em;border-radius:.75em;text-align:center}.active,.btn:active,.btn:visited{background-color:#4564a4!important}.btns{margin:.25em auto;display:grid;grid-gap:.25em;grid-template-columns:repeat(auto-fit,minmax(120px,1fr))}.btn-sm,.btns-sm,.sp{grid-template-columns:repeat(auto-fit,minmax(60px,1fr))}.btns-bg{grid-template-columns:repeat(auto-fit,minmax(74px,1fr));margin-top:1em}.btns.red button{background-color:#df513b;color:#fff}.btns-sm button{font-size:.88rem}.sp button{background:#e7e3c1;color:#0f2c4b;font-size:.8rem}.ex{background:#19d191;color:#fff}div#app span{width:3em;height:1.3em;background:#e7e3c1;color:navy;text-align:center;padding:.3em;margin:auto;border-radius:.5em}img,svg{display:grid;align-content:center;justify-content:space-around;max-width:10em;margin:auto} 
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

<div class="col">
  <h3 title="ESP32 A1S I2S V2.2">@radio RNŚ</h3>
  <div id="info"></div>
  <div id="app">
    <div class="btns btn-sm blue">
      <button class="btn" onClick='sn("radio?v=m")'>Vol-</button>
      <span id="vol"></span>
      <button class="btn" onClick='sn("radio?v=p")'>Vol+</button>
    </div>
    <div class="btns btn-sm red">
      <button class="btn" onClick='sn("radio?s=m")'>Sta-</button>
      <span id="sta"></span>
      <button class="btn" onClick='sn("radio?s=p")'>Sta+</button>
    </div>   
  </div>
  <div class="btns btns-bg btns-sm red st">
    <button onClick='sn("radio?t=0")' class="btn" id="st0">TOK-FM</button>
    <button onClick='sn("radio?t=1")' class="btn" id="st1">RNŚ</button>
    <button onClick='sn("radio?t=2")' class="btn" id="st2">357</button>
    <button onClick='sn("radio?t=3")' class="btn" id="st3">Sw-Jzz</button>
    <button onClick='sn("radio?t=4")' class="btn" id="st4">Sw-Cla</button>
    <button onClick='sn("radio?t=5")' class="btn" id="st5">Sw-Pop</button>
    <button onClick='sn("radio?t=6")' class="btn" id="st5">Cl-FM</button>
    <button onClick='sn("radio?t=7")' class="btn" id="st5">101-J</button>

  </div>
  <div class="btns btns-bg btns-sm blue">
    <button onClick='sn("radio?n=0")' class="btn ex">Info</button>
    <button onClick='sn("radio?start=0")' class="btn ex">Mute</button>
    <button onClick='sn("radio?z=0")' class="btn ex">Reset</button>
  </div>
</div>

<hr />
  <div class="col btns sp">
    <button onClick='sn("radio?q=0")' class="btn" >eq(0)</button>
    <button onClick='sn("radio?q=1")' class="btn" >eq(1)</button>
    <button onClick='sn("radio?q=2")' class="btn" >eq(2)</button>
    <button onClick='sn("radio?q=3")' class="btn" >eq(3)</button>
    <button onClick='sn("radio?q=4")' class="btn" >eq(4)</button>
    <button onClick='sn("radio?q=5")' class="btn" >eq(5)</button>
    <button onClick='sn("radio?q=6")' class="btn" >eq(6)</button>
    <button onClick='sn("radio?q=7")' class="btn" >eq(7)</button>
    <button onClick='sn("radio?q=8")' class="btn" >eq(8)</button>
    <button onClick='sn("radio?q=9")' class="btn" >eq(9)</button>
  </div>


<!--img src="radio.svg" /-->
</body>
</html>


)=====";
