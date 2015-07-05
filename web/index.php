<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8">
		<link rel="stylesheet" href="jquery-ui/jquery-ui.min.css">
		<link rel="stylesheet" href="index.css">
		<script src="jquery-2.1.4.min.js"></script>
		<script src="jquery-ui/jquery-ui.min.js"></script>
		<script src="index.js"></script>
		<title>DRD v0.1a</title>
	</head>
	<body>
		<h3>Status</h3>
		<div id="status"></div>
		<div>
			<h3>Controls</h3>
			<div>
				<div>Direction Control</div>
				<button id="stop">Stop</button>
				<button id="forward">Forward</button>
				<button id="left">Left</button>
				<button id="right">Right</button>
				<button id="reverse">Reverse</button>
			</div>
		</div>
		<div>
			<div>Speed Settings</div>
				<div>Forward<div class="slider" id="forwardSpeed"></div></div>
				<div>Turn<div class="slider" id="turnSpeed"></div></div>
			</div>
		</div>
		<div>
			<h3>Camera</h3>
			<button id="camera">Get Picture</button>
			<a class="toggler" id="autoRefresh">&nbsp;</a>Auto-refresh
		</div>
		<div>
			<img src="" width="320" height="240"/>
		</div>
		<!-- video autoplay="true" controls width="320" height="240">
			<source src="http://192.168.0.116:8554/stream" />
			Could not use HTML5 video.
		</video -->
	</body>
</html>
