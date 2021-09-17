const char PAGE_HTML[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="pl">
<head>
<meta charset="utf-8">
<title>@radio I2S</title>
<meta name="viewport" content="width=device-width,initial-scale=1">
<meta name="mobile-web-app-capable" content="yes">
<meta name="theme-color" content="#db5945">
<style>
html {font:normal 1.05em verdana}
body {background-color: #152028; padding: 0.25em; color:white;}
.col {max-width: 600px;margin: auto; text-align:center;}
h3 {margin-top:0;}
#info,#czas {min-height: 1.5em;font-size:0.8em;}
.btn {
    background-color: #202945;
    border-color: #000;
    color: white;
    padding: 1em 0.25em;
    box-shadow: inset 0px 0px 3px 3px #16162f;
  border-radius: 0.75em;
  text-align: center;
}
.btn:active, .btn:visited, .active {
    background-color: #2c395f;
    border-color: #6079c7;
    box-shadow: inset 0px 0px 3px 3px #4b4ba7;
}
.btns {
  max-width: 1024px;
  margin: 0.5em auto;
  display: grid;
  grid-gap: 0.5em;
  grid-template-columns: repeat(auto-fit, minmax(120px, 1fr));
}
.btns-sm {
  grid-template-columns: repeat(auto-fit, minmax(60px, 1fr));
}
.btns-bg {
  grid-template-columns: repeat(auto-fit, minmax(80px, 1fr));
}
.btns-sm .btn {font-size:0.95em; background-color:#182345;}
.btns.red button{background-color:#400;}
.btns.blue button{background-color:#1b3070;}
div#app span {
    width: 3em;
    height: 1.3em;
    background: white;
    color: navy;
    text-align: center;
    padding: 0.3em;
    margin: auto;
    border-radius: 0.5em;
}
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
  <h3>@radio I2S NŚ</h3>
  <div id="info"></div>
  <div id="app">
  <div class="btns btns-sm blue">
    <button class="btn" onClick='sn("radio?s=m")'>Sta-</button>
    <span id="sta"></span>
    <button class="btn" onClick='sn("radio?s=p")'>Sta+</button>
  </div>
  <div class="btns btns-sm red">
    <button class="btn" onClick='sn("radio?v=m")'>Vol-</button>
    <span id="vol"></span>
    <button class="btn" onClick='sn("radio?v=p")'>Vol+</button>
  </div>
  </div>
  <div class="btns btns-bg blue">
    <button onClick='sn("radio?n=0")' class="btn">Info</button>
    <button onClick='sn("radio?start=0")' class="btn">Mute</button>
    <button onClick='sn("radio?z=0")' class="btn">Reset</button>
  </div>

</div>
</body>
</html>


)=====";
