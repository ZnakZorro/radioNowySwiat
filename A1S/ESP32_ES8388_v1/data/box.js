/* js.js START*/
console.log("##js.js");
//const _$=e=>document.querySelector(e);
//const _$$=e=>document.querySelectorAll(e);

const setTone=(a,b,c)=>{
	console.log(a,b,c);
	fetch(url+"radio?eql="+a+"&eqm="+b+"&eqh="+c).then(r => {return r.text()}).then(o => {
		opisz(o);
		console.log(o);
		fetch(url+"radio?eqq=1");
	}).catch(e => {relo(e);})
}
	
const playMP3=(m)=>{
	console.log(m);
	fetch(url+"radio?mp3="+m).then(r => {return r.text()}).then(o => {
		opisz(o);
		console.log(o);
	}).catch(e => {relo(e);})
}
const hidee=(t)=>{t.style.display="none";} 
const hide=(t)=>{document.querySelector(t).style.display="none";} 

const installEQ=()=>{
	console.log('installEQ');
	hide("#testy");
	hide("#pre");
	let eq = _$("#eq");
	console.log(eq);
	let eqeq=setGains(4);
	console.log(eqeq);
	let html ="<div>\n";
	eqeq.forEach((e)=>{
		html += `<button ondblclick="setTone(${e[0]},${e[1]},${e[2]})">${e[0]},${e[1]},${e[2]}</button>\n`;
	});
	html += "<p> /mp3/ </p>\n";
	arrFRQmp3.forEach((e)=>{
		html += `<button ondblclick="playMP3(\'${e}\')">${e}</button>\n`;
	});
	html += "<p> /mp3/a/ </p>\n";
	muzyka.forEach((e)=>{
		html += `<button ondblclick="playMP3(\'a/${e}\')">a/${e}</button>\n`;
	});
	html += "</div>\n";
	eq.innerHTML = html;
}



const setGains=(q)=>{
	let arr =[];
	for(let x=-1; x<2; x++)
		for(let y=-1; y<2; y++)
			for(let z=-1; z<2; z++)
				arr.push([x*q,y*q,z*q]);		
	return arr;		
}



let arrFRQmp3=[
"00060.mp3",
"00100.mp3",
"00250.mp3",
"00440.mp3",
"01000.mp3",
"04000.mp3",
"125-16000.mp3",
"16000.mp3",
"test.mp3",
"pink.mp3",
"sweep.mp3",
"white.mp3"
];

let muzyka=[
"01.Przasniczki.mp3",
"02.Oberek.mp3",
"03.Cyt-cyt.mp3",
"04.To-i-hola.mp3",
"05.Dwa-serduszka-cztery-oczy.mp3",
"07.Kukuleczka kuka.mp3",
"08.W-olszynie.mp3",
"09.Kadzioleczka.mp3",
"10.Wierzba.mp3",
"11.Mazurek-F-dur.mp3"
];



/* js.js EOF*/
/* iir.js START*/
console.log("##iir.js");

let slajdery=[
	{"name":"qainL", "ids":"eql","idf":"field1", "min":-12,  "max":6,     "val":0,    "onChange":"baar"},	
	{"name":"qainM", "ids":"eqm","idf":"field2", "min":-12,  "max":6,     "val":0,    "onChange":"baar"},	
	{"name":"qainH", "ids":"eqh","idf":"field3", "min":-12,  "max":6,     "val":0,    "onChange":"baar"},	
	{"name":"frqL",  "ids":"slider4","idf":"field4", "min":20,   "max":1000,  "val":300,  "onChange":"baar"},	
	{"name":"frqM",  "ids":"slider5","idf":"field5", "min":500,  "max":4000,  "val":1000, "onChange":"baar"},	
	{"name":"frqH",  "ids":"slider6","idf":"field6", "min":2000, "max":16000, "val":4000, "onChange":"baar"},	
	{"name":"Q",     "ids":"slider7","idf":"field7", "min":1,    "max":50,    "val":7,    "onChange":"baar"},
];

let iirOBJ = {"egl":0,"eqm":0,"eqh":0};

let baar=(t)=>{
	let id    = t.id;
	let outID = t.dataset.out;
	let val   = t.value;
	document.getElementById(outID).value=val;
	iirOBJ[id]=val;
	console.log(id,outID,val,iirOBJ);
	setTone(iirOBJ.eql,iirOBJ.eqm,iirOBJ.eqh);
}

const installIIR=()=>{
	console.log('installIIR');
	hide("#testy");
	hide("#pre");
	let eq = _$("#eq");
	console.log(eq);
	let html = '<div id="sliders">';
	slajdery.forEach((s)=>{
		html += `
				<div>
				<span class="name">${s.name}</span>
				<input id="${s.idf}" type="text" class="field" value="${s.val}" />
				<input id="${s.ids}" type="range" class="slider" 
				min="${s.min}" max="${s.max}" value="${s.val}" 
				data-out="${s.idf}" 
				onChange="${s.onChange}(this)" 
				/>
				
				</div>
				`;
	});
	html += "</div>";
	eq.innerHTML = html;
}






/* iir.js EOF*/console.log("##char.js");


