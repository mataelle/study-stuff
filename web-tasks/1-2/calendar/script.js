$(document).ready(function () {

	var div = function(a,b) { return (a-a%b)/b; }
	var getNumRows = function (year, month) {
		var firstDay = new Date(year, month,1);
		var lastDay = new Date(year, month+1,0);
		var tabFirst = (firstDay.getDay() +1) % 7;
		console.log((tabFirst > 0 ? 1 : 0), div(lastDay.getDate()-tabFirst,7) , ((lastDay.getDay()+1)%7 > 0 ? 1 : 0));
		return (tabFirst > 0 ? 1 : 0) + div(lastDay.getDate()-(7-tabFirst),7) + ((lastDay.getDay()+1)%7 > 0 ? 1 : 0);
	}
	var weekDays = ["Понедельник", "Вторник","Среда","Четверг","Пятница","Суббота","Воскресенье"];
	var months = ["Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"]

	var today = new Date();
	var weekDay = today.getDay() > 0 ? today.getDay()-1 : 6;
	var weekNum = div(today.getDate(),7);

	var fillCalendar = function(curYear, curMonth) {
		$("#month-year").text(months[curMonth]+" "+String(curYear));
		console.log($('.table').children);
        // $('#body-container>.holder').remove($('.table'));
        $('.table').remove();
		$('#body-container>.holder').append($('<div class="table">'));
	    for (i=0;i<getNumRows(curYear, curMonth);i++){
	        $('.table').append($('<div class="row-'+String(i)+'">'));
	        for (j=0;j<7;j++) {
	        		if (today.getMonth()==curMonth && today.getFullYear()==curYear) {
		        		toIns = new Date(today.getFullYear(), today.getMonth(),today.getDate()+(i*7+j)-(weekNum*7+weekDay));
		            	$('.row-'+String(i)).append($('<div class="for-selection'+((i==weekNum && j == weekDay)? " today" :"")+'">')
		            		.append($('<div class="cell'+ ((i==weekNum && j == weekDay)? " today" :"")+ '">').text((i==0? weekDays[j]+", " : "")+String(toIns.getDate()))));
	            	}
	            	else {
	            		tempFirst = new Date(curYear, curMonth,1);
		        		toIns = new Date(curYear, curMonth,1+(i*7+j)-(tempFirst.getDay() > 0 ? tempFirst.getDay()-1 : 6));
		            	$('.row-'+String(i)).append($('<div class="for-selection">')
		            		.append($('<div class="cell">').text((i==0? weekDays[j]+", " : "")+String(toIns.getDate()))));	            		
	            	}
	        }
	    }

	}

	var curMonth = today.getMonth();
	var curYear = today.getFullYear();
	fillCalendar(curYear, curMonth);

    $('.for-selection').click(function() {
    	$(".selected").toggleClass('selected');
    	$(".inner-selected").toggleClass('inner-selected');

    	$(this).toggleClass('selected');
    	$($(this).children()[0]).toggleClass('inner-selected');

    	//pop-up
    });
    $('.btn').click(function(){
    	$(this).toggleClass('is-pressed');
    });
    $('#month-less').click(function(){
    	curMonth-=1;
    	if (curMonth == -1) {
    		curYear -=1;
    		curMonth = 11;
    	}
		fillCalendar(curYear, curMonth);
    });
    $('#month-more').click(function(){
    	curMonth+=1;
    	if (curMonth == 12) {
    		curYear +=1;
    		curMonth = 0;
    	}
		fillCalendar(curYear, curMonth);
    });
    $('#today').click(function() {
		curMonth = today.getMonth();
		curYear = today.getFullYear();
		fillCalendar(curYear, curMonth);
    });
    $('#today').mouseup(function(){
    	$(this).toggleClass("is-pressed-small");    	
    });

    $('#today').mousedown(function(){
    	$(this).toggleClass("is-pressed-small");    	
    });

});