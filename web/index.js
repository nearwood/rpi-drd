$(function() {
	$("button").button();
	$(".slider").slider();
	//$("input[type=checkbox]").button();

	$("#stop").click(function() {
		$.ajax("cgi-bin/test").done(function(data, status) {
			$("#status").text(status + ":" + data);
		}).fail(function(data, status) {
			$("#status").text(status + ":" + data);
		});

		return false; //avoid page reload
	});


});
