var audioContext;
var analyser;
var player;
var filesCounter = 0;
var songsCounter = 0;

$(document).ready(function() {

	//checking up

	try {
		audioContext = new AudioContext();
	} catch(e) {
		$("#fatal-error").text("Sorry! This browser does not support the Web Audio API. Please use Chrome, Safari, Firefox or Yandex browser.");
		return;
	}
    var div = document.createElement('div');
	if (!('draggable' in div || ('ondragstart' in div && 'ondrop' in div) )){
		$('#fatal-error').text("Whoa! This browser does not support drag'n'drop.");
		return;
	}

	$('#hint').text("Hint: you can change the order of songs in your playlist. To start playing from another song click on it.");
	$('#hint').css('display', 'block');
	$("label").css('display', 'inline-block');
	$("#file-dropper").css('display', 'inline-block');

	//visualization

	var cc = document.getElementById('canvas');
	var canvas = cc.getContext("2d");
	var gradient = canvas.createLinearGradient(0,0,200,0);
    gradient.addColorStop(1,'#3df');
    gradient.addColorStop(0.75,'#3fc');
    gradient.addColorStop(0.25,'#fdc');
    gradient.addColorStop(0,'#f13');

	processNode = audioContext.createScriptProcessor(2048, 1, 1);
	processNode.connect(audioContext.destination);

	analyser = audioContext.createAnalyser();
    analyser.smoothingTimeConstant = 0.3;
    analyser.fftSize = 1024;
	analyser.connect(processNode);

    sourceNode = audioContext.createBufferSource();
    sourceNode.connect(analyser);
    analyser.connect(processNode);
    sourceNode.connect(audioContext.destination);

    processNode.onaudioprocess = function() {
        var array =  new Uint8Array(analyser.frequencyBinCount);
        analyser.getByteFrequencyData(array);
        var width = window.innerWidth;
        var height = window.innerHeight-100;
       	canvas.clearRect(0, 0, width, height);
        canvas.width = width;
		canvas.fillStyle=gradient;
        for (var i = 0; i <array.length; i++){
            canvas.fillRect(i*6,300-array[i],4,300);
        }
    }

    // 

	player = new Player();
	var playlist = document.querySelectorAll('#songs > div');
	var movingElement;
	$(window).load(function () {
		if ( $(window).width() > 800 ) $('#player').css('width', $(window).width()-400);
	});
	$(window).resize(function () {
		if ( $(window).width() > 800 ) $('#player').css('width', $(window).width()-400);
	});

	// processing actions while adding files

	function loadFiles(files) {
		$('#file-dropper').text("Or just drop them here.");
		filesCounter+=files.length;
		for (i=0;i<files.length;i++) {
			var id = "song"+(songsCounter+i+1);
			var reader = new FileReader();
			$('#loading-state').text("Loading music...");
			reader.onload = (function (file, id) {
					return function(evt) {
						var data = evt.target.result;
							ID3.loadTags(file.name, function() { player.decode(data, id, file.name, ID3.getAllTags(file.name)); }, 
							{ tags: ["title","artist","album"], dataReader: FileAPIReader(file) });
					};
			})(files[i], id);	
			reader.readAsArrayBuffer(files[i]);
		}
	}

	document.getElementById('header').addEventListener('dragover', function(e) { 
		e.preventDefault();
		$('#file-dropper').text("Yes, right here.");
	 }, false);
	document.getElementById('header').addEventListener('dragleave', function(e) { 
		e.preventDefault();
		$('#file-dropper').text("Or just drop them here.");
	 }, false);
	document.getElementById('header').addEventListener('drop', function(e) {
		e.preventDefault();
		e.stopPropagation();
		loadFiles(e.dataTransfer.files);
	}, false);
	document.getElementById('file-picker').addEventListener('change', function(e) {
		loadFiles(e.target.files);
	}, false);

	$('.play').on('click', function(){
		$(this).toggleClass('pause');
		if ($(this).attr('class').indexOf('pause') > -1)
			player.play();		
		else
			player.pause();
	}); 
	$('.stop').on('click', function(){
		if ($('.play').attr('class').indexOf('pause') > -1)
			$('.play').removeClass('pause');
		player.stop();
	});
});


// player

function appendSong(id, name) {
	$("#player").css('display', 'inline-block');
	$('#songs').append('<div id="'+ id + '" draggable="true">'+ name+'</div>');
	document.getElementById(id).addEventListener('dragstart', function(e) {
		movingElement = $(this);
		$(this).css("opacity", "0.5");
		e.dataTransfer.effectAllowed = 'move';
		e.dataTransfer.setData('text/html', this.innerHTML);
	}, false);
	document.getElementById(id).addEventListener('dragenter', function(e) { $(this).addClass('over'); }, false);
	document.getElementById(id).addEventListener('dragleave', function(e) { $(this).removeClass('over'); }, false);
	document.getElementById(id).addEventListener('dragover', function(e) {
		if (e.preventDefault) e.preventDefault(); 
		e.dataTransfer.dropEffect = 'move'; 
		$(this).addClass('over');
		return false;
	}, false);

	document.getElementById(id).addEventListener('drop', function(e) {
		if (e.stopPropagation) e.stopPropagation(); 
		if (movingElement != $(this)) {
			var inserted = false;
			for (i=0; i<playlist.length; i++) {
				if (!inserted && $(playlist[i]).attr('id') == movingElement.attr('id')){
					movingElement.insertAfter($(this));
					inserted = true;
				}
				else if (!inserted && $(playlist[i]).attr('id') == $(this).attr('id')){
					$(this).insertAfter(movingElement);
					inserted = true;
				}
			}
			playlist = document.querySelectorAll('#songs > div');
		}
		return false;
	}, false);
	document.getElementById(id).addEventListener('dragend', function(e) {
		movingElement.css("opacity",'1');
		for (i=0; i<playlist.length; i++) ($(playlist[i])).removeClass('over');
	},false);
	document.getElementById(id).addEventListener('click', function(e) { 
		player.start($(this).attr('id')); 
	}, false);
	playlist = document.querySelectorAll('#songs > div');
}



function Player () {
	this.player = $('#player');
	this.songs = {};
	this.position = 0;
	this.startTime = 0;
}

Player.prototype.decode = function(arrayBuffer, songId, songName, metadata) {
	songsCounter += 1;
	audioContext.decodeAudioData(arrayBuffer, function(audioBuffer) {
		filesCounter -= 1;
		if (filesCounter == 0) {
			$('#loading-state').text("");
		}
		this.songs[songId] = {};
		this.songs[songId]["buffer"] = audioBuffer;
		this.songs[songId]["name"] = songName;
		this.songs[songId]["artist"] = metadata.artist;
		this.songs[songId]["album"] = metadata.album;
		this.songs[songId]["title"] = metadata.title;
		if (!this.playing) {
			this.currentSongId = songId;
			this.play(songId);			
		}
		appendSong(songId, songName);	
	}.bind(this), function(err) {
		$('#loading-state').text("Oops! wrong file format.");
		filesCounter -= 1;
		if (filesCounter == 0) {
			$('#loading-state').text("");
		}
	});
};
Player.prototype.play = function() {
	if (this.playing) this.stop();
	$('#now-playing').text(this.songs[this.currentSongId]["name"]);
	$('#title').text("Title: "+this.songs[this.currentSongId]["title"]);
	$('#artist').text("Artist: "+this.songs[this.currentSongId]["artist"]);
	$('#album').text("Album: "+this.songs[this.currentSongId]["album"]);

	if($(document.getElementsByClassName('play')[0]).attr('class').indexOf('pause') < 0) 
		$(document.getElementsByClassName('play')[0]).addClass('pause');

	this.currentPlaying = audioContext.createBufferSource();
	this.currentPlaying.connect(analyser);
	this.currentPlaying.buffer = this.songs[this.currentSongId].buffer;
	this.currentPlaying.connect(audioContext.destination);
	this.currentPlaying.onended = function(e) {
		if (player.playing && audioContext.currentTime - player.startTime > 1) {
			var i=0;
			var id = player.currentSongId;
			while (i<playlist.length) {
				if (id == $(playlist[i]).attr('id')){
					player.currentSongId = $(playlist[ i+1 < playlist.length ? i+1 : 0 ]).attr('id');
					player.play();
					break;
				}
				else i++;
			}
		}
	}
	this.startTime = audioContext.currentTime-this.position;
	this.currentPlaying.start(audioContext.currentTime, this.position);
	this.playing = true;
};
Player.prototype.start = function (id) {
	this.stop();
	this.currentSongId = id;
	this.play();
};
Player.prototype.pause = function() {
	this.position = audioContext.currentTime - this.startTime;
	this.playing = false;
	this.currentPlaying.stop(0);
};
Player.prototype.stop = function() {
	this.position = 0;
	this.playing = false;
	this.currentPlaying.stop(0);
};


