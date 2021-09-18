const char PAGE_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pl">
<head>
<meta charset="utf-8">
<title>@radio RNS</title>
<meta name="viewport" content="width=device-width,initial-scale=1">
<meta name="mobile-web-app-capable" content="yes">
<meta name="theme-color" content="#0F2C4B">
<style>
  /*#4B110F#8D9176#0F2C4B#E7E3C1#D67E2B*/
html {font:normal 14pt verdana}
body {background-color: #0F2C4B; padding: 0.25em; color:white;}
.col {max-width: 600px;margin: auto; text-align:center;}
h3 {margin:0.3em 0;}
#info,#czas {font-size:0.85em;font-family: monospace;}
#info {min-height: 4.5em;}
.btn {
  background-color: #28396f;
  border-color: #000;
  color: white;
  padding: 0.5em 0.25em;
  box-shadow: inset 0px 0px 1px 1px #16162f;
  border-radius: 0.75em;
  text-align: center;
}
.btn:active, .btn:visited, .active {
  background-color: #2c395f!important;
  border-color: #6079c7;
  box-shadow: inset 0px 0px 2px 2px #4b4ba7;
}
.btns {
  max-width: 1024px;
  margin: 0.5em auto ;
  display: grid;
  grid-gap: 0.5em;
  grid-template-columns: repeat(auto-fit, minmax(120px, 1fr));
}
.btns-sm,.btn-sm {grid-template-columns: repeat(auto-fit, minmax(60px, 1fr));}
.btns-bg {grid-template-columns: repeat(auto-fit, minmax(80px, 1fr));margin-top:1em;}

.btns.red button{background-color:#4B110F;}

div#app span {
  width: 3em;
  height: 1.3em;
  background: #E7E3C1;
  color: navy;
  text-align: center;
  padding: 0.3em;
  margin: auto;
  border-radius: 0.5em;
}
span.sp5 {padding:1em; display: grid;grid-template-columns: repeat(auto-fit, minmax(80px, 1fr));border: 1px solid black; box-shadow: inset 0px 0px 2px 2px black;}
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
}).catch(e => {console.log(e)})  
}

const btnActive=(lSTA)=>{
  _$$("div.st button").forEach((b)=>{if (b) b.classList.remove("active")});
  console.log(_$("#st"+lSTA));
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
  <div class="btns btns-bg btns-sm blue">
    <button onClick='sn("radio?n=0")' class="btn">Info</button>
    <button onClick='sn("radio?start=0")' class="btn">Mute</button>
    <button onClick='sn("radio?z=0")' class="btn">Reset</button>
  </div>
  <div class="btns btns-bg btns-sm red st">
    <button onClick='sn("radio?t=0")' class="btn" id="st0">TOK-FM</button>
    <button onClick='sn("radio?t=1")' class="btn" id="st1">RNŚ</button>
    <button onClick='sn("radio?t=2")' class="btn" id="st2">357</button>
    <button onClick='sn("radio?t=3")' class="btn" id="st3">S-Jazz</button>
    <button onClick='sn("radio?t=4")' class="btn" id="st4">S-Class</button>
    <button onClick='sn("radio?t=5")' class="btn" id="st5">RMF-Cl</button>

  </div>

</div>
  
<br /><br /><br /> 
<!--#4B110F#8D9176#0F2C4B#E7E3C1#D67E2B-->
  <div class="col btns">
    <span class="sp5" style="background:#4B110F">#4B110F</span>
    <span class="sp5" style="background:#8D9176">#8D9176</span>
    <span class="sp5" style="background:#0F2C4B">#0F2C4B</span>
    <span class="sp5" style="background:#E7E3C1">#E7E3C1</span>
    
  </div>
</body>
</html>



)=====";
