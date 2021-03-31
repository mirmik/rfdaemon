
const char* defaultDevDescFile =
"{\r\n\
	\"loglevel\": \"debug\",\r\n\
	\"idn\": \"RADIOLINE,pos200,OPU0312_xr320,1.2.1\",\r\n\
	\"paths\": {\r\n\
		\"mrs\": \"/dev/ttyS3\",\r\n\
		\"laz\": \"/dev/ttyS1\",\r\n\
		\"drv\": \"/dev/ttyS0\"\r\n\
	},\r\n\
	\"ports\": {\r\n\
		\"scpi\": 5025,\r\n\
		\"ncpi\": 5026,\r\n\
		\"data\": 5802\r\n\
	},\r\n\
	\"axes\": [\r\n\
		{\r\n\
			\"name\": \"Az\",\r\n\
			\"type\": \"sync_axis\",\r\n\
			\"motionType\": \"round\",\r\n\
			\"servo\": \"srv_X\",\r\n\
			\"unitRatio\": 34339,\r\n\
			\"defaultSpeed_rpm\": 200,\r\n\
			\"defaultAccel_ms\": 5000,\r\n\
			\"maxSpeed_rpm\": 500,\r\n\
			\"minAccel_ms\": 1000,\r\n\
			\"backwardLimit\": -180,\r\n\
			\"forwardLimit\": 180,\r\n\
			\"reverse\": 0,\r\n\
			\"syncro\": \"sync_Az\"\r\n\
		},\r\n\
		{\r\n\
			\"name\": \"El\",\r\n\
			\"type\": \"simple\",\r\n\
			\"motionType\": \"round\",\r\n\
			\"servo\": \"srv_axis_2\",\r\n\
			\"unitRatio\": 2056,\r\n\
			\"defaultSpeed_rpm\": 202,\r\n\
			\"defaultAccel_ms\": 5000,\r\n\
			\"maxSpeed_rpm\": 300,\r\n\
			\"minAccel_ms\": 1000,\r\n\
			\"backwardLimit\": -60,\r\n\
			\"forwardLimit\": 60,\r\n\
			\"reverse\": 0\r\n\
		},\r\n\
		{\r\n\
			\"name\": \"Sl\",\r\n\
			\"type\": \"simple\",\r\n\
			\"motionType\": \"linear\",\r\n\
			\"servo\": \"srv_axis_3\",\r\n\
			\"unitRatio\": 2860,\r\n\
			\"defaultSpeed_rpm\": 202,\r\n\
			\"defaultAccel_ms\": 1000,\r\n\
			\"maxSpeed_rpm\": 1500,\r\n\
			\"minAccel_ms\": 1000,\r\n\
			\"backwardLimit\": -70,\r\n\
			\"forwardLimit\": 70,\r\n\
			\"reverse\": 1\r\n\
		},\r\n\
		{\r\n\
			\"name\": \"Sl2\",\r\n\
			\"type\": \"simple\",\r\n\
			\"motionType\": \"linear\",\r\n\
			\"servo\": \"srv_axis_4\",\r\n\
			\"unitRatio\": 27354,\r\n\
			\"defaultSpeed_rpm\": 202,\r\n\
			\"defaultAccel_ms\": 1000,\r\n\
			\"maxSpeed_rpm\": 3000,\r\n\
			\"minAccel_ms\": 1000,\r\n\
			\"backwardLimit\": -70,\r\n\
			\"forwardLimit\": 70,\r\n\
			\"reverse\": 0\r\n\
		},\r\n\
		{\r\n\
			\"name\": \"Pol\",\r\n\
			\"type\": \"simple\",\r\n\
			\"motionType\": \"round\",\r\n\
			\"servo\": \"srv_axis_5\",\r\n\
			\"unitRatio\": 2056,\r\n\
			\"defaultSpeed_rpm\": 202,\r\n\
			\"defaultAccel_ms\": 5000,\r\n\
			\"maxSpeed_rpm\": 300,\r\n\
			\"minAccel_ms\": 1000,\r\n\
			\"backwardLimit\": -100,\r\n\
			\"forwardLimit\": 100,\r\n\
			\"reverse\": 0\r\n\
		},\r\n\
		{\r\n\
			\"name\": \"Pol2\",\r\n\
			\"type\": \"simple\",\r\n\
			\"motionType\": \"round\",\r\n\
			\"servo\": \"srv_axis_6\",\r\n\
			\"unitRatio\": 2056,\r\n\
			\"defaultSpeed_rpm\": 202,\r\n\
			\"defaultAccel_ms\": 1000,\r\n\
			\"maxSpeed_rpm\": 1500,\r\n\
			\"minAccel_ms\": 1000,\r\n\
			\"backwardLimit\": -100,\r\n\
			\"forwardLimit\": 100,\r\n\
			\"reverse\": 0\r\n\
		}\r\n\
	],\r\n\
	\"devices\": [\r\n\
		{\r\n\
			\"name\": \"srv_X\",\r\n\
			\"type\": \"mitsuservo_type_A\",\r\n\
			\"encoderType\": \"internal\",\r\n\
			\"address\": 2,\r\n\
			\"updateTime\": 50,\r\n\
			\"PA\": [\r\n\
				{\r\n\
					\"name\": \"PA01\",\r\n\
					\"value\": \"1000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA02\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA03\",\r\n\
					\"value\": \"0002\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA04\",\r\n\
					\"value\": \"2000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA05\",\r\n\
					\"value\": \"10000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA06\",\r\n\
					\"value\": \"100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA07\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA08\",\r\n\
					\"value\": \"0002\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA09\",\r\n\
					\"value\": \"16\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA10\",\r\n\
					\"value\": \"300\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA11\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA12\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA13\",\r\n\
					\"value\": \"0112\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA14\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA15\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA16\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA17\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA18\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA19\",\r\n\
					\"value\": \"00AA\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA20\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA21\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA22\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA25\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				}\r\n\
			],\r\n\
			\"PB\": [\r\n\
				{\r\n\
					\"name\": \"PB01\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB02\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB03\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB04\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB05\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB06\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB07\",\r\n\
					\"value\": \"15.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB08\",\r\n\
					\"value\": \"37.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB09\",\r\n\
					\"value\": \"823\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB10\",\r\n\
					\"value\": \"33.7\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB11\",\r\n\
					\"value\": \"980\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB12\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB13\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB14\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB15\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB16\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB17\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB18\",\r\n\
					\"value\": \"3141\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB19\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB20\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB21\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB22\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB25\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB27\",\r\n\
					\"value\": \"10\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB28\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB29\",\r\n\
					\"value\": \"7.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB30\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB32\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB33\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB34\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB35\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB36\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB46\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB47\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB48\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB49\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB50\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB51\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB52\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB53\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB54\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB55\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB56\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB57\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB58\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB59\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB60\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PC\": [\r\n\
				{\r\n\
					\"name\": \"PC01\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC02\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC03\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC04\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC05\",\r\n\
					\"value\": \"100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC06\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC07\",\r\n\
					\"value\": \"1000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC08\",\r\n\
					\"value\": \"200\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC09\",\r\n\
					\"value\": \"300\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC10\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC11\",\r\n\
					\"value\": \"800\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC12\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC13\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC14\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC15\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC16\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC17\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC18\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC19\",\r\n\
					\"value\": \"0010\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC22\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC24\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC27\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC30\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC32\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC33\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC34\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC35\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC36\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC37\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC38\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC39\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC40\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC43\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC44\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC51\",\r\n\
					\"value\": \"100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC54\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC60\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC66\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC67\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC68\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC69\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC70\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC71\",\r\n\
					\"value\": \"0040\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC72\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC73\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PD\": [\r\n\
				{\r\n\
					\"name\": \"PD01\",\r\n\
					\"value\": \"1C04\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD03\",\r\n\
					\"value\": \"0202\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD04\",\r\n\
					\"value\": \"0202\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD05\",\r\n\
					\"value\": \"2100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD06\",\r\n\
					\"value\": \"2021\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD07\",\r\n\
					\"value\": \"0704\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD08\",\r\n\
					\"value\": \"0707\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD09\",\r\n\
					\"value\": \"0805\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD10\",\r\n\
					\"value\": \"0808\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD11\",\r\n\
					\"value\": \"0303\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD12\",\r\n\
					\"value\": \"3803\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD13\",\r\n\
					\"value\": \"2006\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD14\",\r\n\
					\"value\": \"3920\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD17\",\r\n\
					\"value\": \"0A0A\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD18\",\r\n\
					\"value\": \"0A00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD19\",\r\n\
					\"value\": \"0B0B\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD20\",\r\n\
					\"value\": \"0B00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD21\",\r\n\
					\"value\": \"2323\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD22\",\r\n\
					\"value\": \"2B23\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD23\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD24\",\r\n\
					\"value\": \"000C\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD25\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD26\",\r\n\
					\"value\": \"0005\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD28\",\r\n\
					\"value\": \"0002\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD29\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD30\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD31\",\r\n\
					\"value\": \"0100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD32\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD33\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD34\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD41\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD42\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD43\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD44\",\r\n\
					\"value\": \"3A00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD46\",\r\n\
					\"value\": \"3B00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD47\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				}\r\n\
			],\r\n\
			\"PE\": [\r\n\
				{\r\n\
					\"name\": \"PE41\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE44\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE45\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE46\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE47\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE48\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE49\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE50\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PF\": [\r\n\
				{\r\n\
					\"name\": \"PF09\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF15\",\r\n\
					\"value\": \"2000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF18\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF21\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF23\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF25\",\r\n\
					\"value\": \"200\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF34\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF46\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			]\r\n\
		},\r\n\
		{\r\n\
			\"name\": \"srv_axis_2\",\r\n\
			\"type\": \"mitsuservo_type_A\",\r\n\
			\"encoderType\": \"internal\",\r\n\
			\"address\": 6,\r\n\
			\"updateTime\": 50,\r\n\
			\"PA\": [\r\n\
				{\r\n\
					\"name\": \"PA01\",\r\n\
					\"value\": \"1000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA02\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA03\",\r\n\
					\"value\": \"0002\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA04\",\r\n\
					\"value\": \"2000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA05\",\r\n\
					\"value\": \"10000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA06\",\r\n\
					\"value\": \"1000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA07\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA08\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA09\",\r\n\
					\"value\": \"16\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA10\",\r\n\
					\"value\": \"300\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA11\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA12\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA13\",\r\n\
					\"value\": \"0112\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA14\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA15\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA16\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA17\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA18\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA19\",\r\n\
					\"value\": \"00AA\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA20\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA21\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA22\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA25\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				}\r\n\
			],\r\n\
			\"PB\": [\r\n\
				{\r\n\
					\"name\": \"PB01\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB02\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB03\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB04\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB05\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB06\",\r\n\
					\"value\": \"7.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB07\",\r\n\
					\"value\": \"15.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB08\",\r\n\
					\"value\": \"37.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB09\",\r\n\
					\"value\": \"823\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB10\",\r\n\
					\"value\": \"33.7\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB11\",\r\n\
					\"value\": \"980\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB12\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB13\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB14\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB15\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB16\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB17\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB18\",\r\n\
					\"value\": \"3141\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB19\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB20\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB21\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB22\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB25\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB27\",\r\n\
					\"value\": \"10\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB28\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB29\",\r\n\
					\"value\": \"7.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB30\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB32\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB33\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB34\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB35\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB36\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB46\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB47\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB48\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB49\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB50\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB51\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB52\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB53\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB54\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB55\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB56\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB57\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB58\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB59\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB60\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PC\": [\r\n\
				{\r\n\
					\"name\": \"PC01\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC02\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC03\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC04\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC05\",\r\n\
					\"value\": \"100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC06\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC07\",\r\n\
					\"value\": \"1000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC08\",\r\n\
					\"value\": \"200\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC09\",\r\n\
					\"value\": \"300\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC10\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC11\",\r\n\
					\"value\": \"800\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC12\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC13\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC14\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC15\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC16\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC17\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC18\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC19\",\r\n\
					\"value\": \"0010\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC22\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC24\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC27\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC30\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC32\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC33\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC34\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC35\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC36\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC37\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC38\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC39\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC40\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC43\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC44\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC51\",\r\n\
					\"value\": \"100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC54\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC60\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC66\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC67\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC68\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC69\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC70\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC71\",\r\n\
					\"value\": \"0040\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC72\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC73\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PD\": [\r\n\
				{\r\n\
					\"name\": \"PD01\",\r\n\
					\"value\": \"1C04\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD03\",\r\n\
					\"value\": \"0202\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD04\",\r\n\
					\"value\": \"0202\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD05\",\r\n\
					\"value\": \"2100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD06\",\r\n\
					\"value\": \"2021\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD07\",\r\n\
					\"value\": \"0704\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD08\",\r\n\
					\"value\": \"0707\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD09\",\r\n\
					\"value\": \"0805\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD10\",\r\n\
					\"value\": \"0808\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD11\",\r\n\
					\"value\": \"0303\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD12\",\r\n\
					\"value\": \"3803\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD13\",\r\n\
					\"value\": \"2006\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD14\",\r\n\
					\"value\": \"3920\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD17\",\r\n\
					\"value\": \"0A0A\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD18\",\r\n\
					\"value\": \"0A00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD19\",\r\n\
					\"value\": \"0B0B\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD20\",\r\n\
					\"value\": \"0B00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD21\",\r\n\
					\"value\": \"2323\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD22\",\r\n\
					\"value\": \"2B23\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD23\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD24\",\r\n\
					\"value\": \"000C\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD25\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD26\",\r\n\
					\"value\": \"0005\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD28\",\r\n\
					\"value\": \"0002\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD29\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD30\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD31\",\r\n\
					\"value\": \"0100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD32\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD33\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD34\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD41\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD42\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD43\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD44\",\r\n\
					\"value\": \"3A00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD46\",\r\n\
					\"value\": \"3B00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD47\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				}\r\n\
			],\r\n\
			\"PE\": [\r\n\
				{\r\n\
					\"name\": \"PE41\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE44\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE45\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE46\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE47\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE48\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE49\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE50\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PF\": [\r\n\
				{\r\n\
					\"name\": \"PF09\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF15\",\r\n\
					\"value\": \"2000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF18\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF21\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF23\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF25\",\r\n\
					\"value\": \"200\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF34\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF46\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			]\r\n\
		},\r\n\
		{\r\n\
			\"name\": \"srv_axis_3\",\r\n\
			\"type\": \"mitsuservo_type_A\",\r\n\
			\"encoderType\": \"internal\",\r\n\
			\"address\": 3,\r\n\
			\"updateTime\": 50,\r\n\
			\"PA\": [\r\n\
				{\r\n\
					\"name\": \"PA01\",\r\n\
					\"value\": \"1000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA02\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA03\",\r\n\
					\"value\": \"0002\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA04\",\r\n\
					\"value\": \"2000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA05\",\r\n\
					\"value\": \"10000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA06\",\r\n\
					\"value\": \"1000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA07\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA08\",\r\n\
					\"value\": \"0002\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA09\",\r\n\
					\"value\": \"16\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA10\",\r\n\
					\"value\": \"300\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA11\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA12\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA13\",\r\n\
					\"value\": \"0112\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA14\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA15\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA16\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA17\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA18\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA19\",\r\n\
					\"value\": \"00AA\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA20\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA21\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA22\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA25\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				}\r\n\
			],\r\n\
			\"PB\": [\r\n\
				{\r\n\
					\"name\": \"PB01\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB02\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB03\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB04\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB05\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB06\",\r\n\
					\"value\": \"30\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB07\",\r\n\
					\"value\": \"15.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB08\",\r\n\
					\"value\": \"37.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB09\",\r\n\
					\"value\": \"823\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB10\",\r\n\
					\"value\": \"33.7\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB11\",\r\n\
					\"value\": \"980\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB12\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB13\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB14\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB15\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB16\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB17\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB18\",\r\n\
					\"value\": \"3141\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB19\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB20\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB21\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB22\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB25\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB27\",\r\n\
					\"value\": \"10\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB28\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB29\",\r\n\
					\"value\": \"7.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB30\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB32\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB33\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB34\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB35\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB36\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB46\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB47\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB48\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB49\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB50\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB51\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB52\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB53\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB54\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB55\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB56\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB57\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB58\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB59\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB60\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PC\": [\r\n\
				{\r\n\
					\"name\": \"PC01\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC02\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC03\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC04\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC05\",\r\n\
					\"value\": \"100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC06\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC07\",\r\n\
					\"value\": \"1000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC08\",\r\n\
					\"value\": \"200\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC09\",\r\n\
					\"value\": \"300\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC10\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC11\",\r\n\
					\"value\": \"800\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC12\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC13\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC14\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC15\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC16\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC17\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC18\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC19\",\r\n\
					\"value\": \"0010\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC22\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC24\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC27\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC30\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC32\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC33\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC34\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC35\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC36\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC37\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC38\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC39\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC40\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC43\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC44\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC51\",\r\n\
					\"value\": \"100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC54\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC60\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC66\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC67\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC68\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC69\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC70\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC71\",\r\n\
					\"value\": \"0040\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC72\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC73\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PD\": [\r\n\
				{\r\n\
					\"name\": \"PD01\",\r\n\
					\"value\": \"1C04\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD03\",\r\n\
					\"value\": \"0202\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD04\",\r\n\
					\"value\": \"0202\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD05\",\r\n\
					\"value\": \"2100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD06\",\r\n\
					\"value\": \"2021\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD07\",\r\n\
					\"value\": \"0704\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD08\",\r\n\
					\"value\": \"0707\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD09\",\r\n\
					\"value\": \"0805\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD10\",\r\n\
					\"value\": \"0808\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD11\",\r\n\
					\"value\": \"0303\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD12\",\r\n\
					\"value\": \"3803\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD13\",\r\n\
					\"value\": \"2006\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD14\",\r\n\
					\"value\": \"3920\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD17\",\r\n\
					\"value\": \"0A0A\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD18\",\r\n\
					\"value\": \"0A00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD19\",\r\n\
					\"value\": \"0B0B\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD20\",\r\n\
					\"value\": \"0B00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD21\",\r\n\
					\"value\": \"2323\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD22\",\r\n\
					\"value\": \"2B23\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD23\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD24\",\r\n\
					\"value\": \"000C\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD25\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD26\",\r\n\
					\"value\": \"0005\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD28\",\r\n\
					\"value\": \"0002\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD29\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD30\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD31\",\r\n\
					\"value\": \"0100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD32\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD33\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD34\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD41\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD42\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD43\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD44\",\r\n\
					\"value\": \"3A00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD46\",\r\n\
					\"value\": \"3B00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD47\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				}\r\n\
			],\r\n\
			\"PE\": [\r\n\
				{\r\n\
					\"name\": \"PE41\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE44\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE45\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE46\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE47\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE48\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE49\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE50\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PF\": [\r\n\
				{\r\n\
					\"name\": \"PF09\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF15\",\r\n\
					\"value\": \"2000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF18\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF21\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF23\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF25\",\r\n\
					\"value\": \"200\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF34\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF46\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			]\r\n\
		},\r\n\
		{\r\n\
			\"name\": \"srv_axis_4\",\r\n\
			\"type\": \"mitsuservo_type_A\",\r\n\
			\"encoderType\": \"internal\",\r\n\
			\"address\": 4,\r\n\
			\"updateTime\": 50,\r\n\
			\"PA\": [\r\n\
				{\r\n\
					\"name\": \"PA01\",\r\n\
					\"value\": \"1000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA02\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA03\",\r\n\
					\"value\": \"0002\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA04\",\r\n\
					\"value\": \"2000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA05\",\r\n\
					\"value\": \"10000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA06\",\r\n\
					\"value\": \"1000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA07\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA08\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA09\",\r\n\
					\"value\": \"16\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA10\",\r\n\
					\"value\": \"300\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA11\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA12\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA13\",\r\n\
					\"value\": \"0112\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA14\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA15\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA16\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA17\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA18\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA19\",\r\n\
					\"value\": \"00AA\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA20\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA21\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA22\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA25\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				}\r\n\
			],\r\n\
			\"PB\": [\r\n\
				{\r\n\
					\"name\": \"PB01\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB02\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB03\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB04\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB05\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB06\",\r\n\
					\"value\": \"7.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB07\",\r\n\
					\"value\": \"15.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB08\",\r\n\
					\"value\": \"37.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB09\",\r\n\
					\"value\": \"823\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB10\",\r\n\
					\"value\": \"33.7\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB11\",\r\n\
					\"value\": \"980\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB12\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB13\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB14\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB15\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB16\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB17\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB18\",\r\n\
					\"value\": \"3141\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB19\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB20\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB21\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB22\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB25\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB27\",\r\n\
					\"value\": \"10\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB28\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB29\",\r\n\
					\"value\": \"7.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB30\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB32\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB33\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB34\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB35\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB36\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB46\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB47\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB48\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB49\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB50\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB51\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB52\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB53\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB54\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB55\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB56\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB57\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB58\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB59\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB60\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PC\": [\r\n\
				{\r\n\
					\"name\": \"PC01\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC02\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC03\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC04\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC05\",\r\n\
					\"value\": \"100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC06\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC07\",\r\n\
					\"value\": \"1000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC08\",\r\n\
					\"value\": \"200\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC09\",\r\n\
					\"value\": \"300\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC10\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC11\",\r\n\
					\"value\": \"800\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC12\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC13\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC14\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC15\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC16\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC17\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC18\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC19\",\r\n\
					\"value\": \"0010\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC22\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC24\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC27\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC30\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC32\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC33\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC34\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC35\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC36\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC37\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC38\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC39\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC40\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC43\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC44\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC51\",\r\n\
					\"value\": \"100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC54\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC60\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC66\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC67\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC68\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC69\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC70\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC71\",\r\n\
					\"value\": \"0040\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC72\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC73\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PD\": [\r\n\
				{\r\n\
					\"name\": \"PD01\",\r\n\
					\"value\": \"1C04\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD03\",\r\n\
					\"value\": \"0202\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD04\",\r\n\
					\"value\": \"0202\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD05\",\r\n\
					\"value\": \"2100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD06\",\r\n\
					\"value\": \"2021\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD07\",\r\n\
					\"value\": \"0704\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD08\",\r\n\
					\"value\": \"0707\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD09\",\r\n\
					\"value\": \"0805\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD10\",\r\n\
					\"value\": \"0808\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD11\",\r\n\
					\"value\": \"0303\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD12\",\r\n\
					\"value\": \"3803\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD13\",\r\n\
					\"value\": \"2006\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD14\",\r\n\
					\"value\": \"3920\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD17\",\r\n\
					\"value\": \"0A0A\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD18\",\r\n\
					\"value\": \"0A00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD19\",\r\n\
					\"value\": \"0B0B\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD20\",\r\n\
					\"value\": \"0B00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD21\",\r\n\
					\"value\": \"2323\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD22\",\r\n\
					\"value\": \"2B23\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD23\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD24\",\r\n\
					\"value\": \"000C\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD25\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD26\",\r\n\
					\"value\": \"0005\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD28\",\r\n\
					\"value\": \"0002\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD29\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD30\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD31\",\r\n\
					\"value\": \"0100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD32\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD33\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD34\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD41\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD42\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD43\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD44\",\r\n\
					\"value\": \"3A00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD46\",\r\n\
					\"value\": \"3B00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD47\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				}\r\n\
			],\r\n\
			\"PE\": [\r\n\
				{\r\n\
					\"name\": \"PE41\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE44\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE45\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE46\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE47\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE48\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE49\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE50\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PF\": [\r\n\
				{\r\n\
					\"name\": \"PF09\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF15\",\r\n\
					\"value\": \"2000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF18\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF21\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF23\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF25\",\r\n\
					\"value\": \"200\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF34\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF46\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			]\r\n\
		},\r\n\
		{\r\n\
			\"name\": \"srv_axis_5\",\r\n\
			\"type\": \"mitsuservo_type_A\",\r\n\
			\"encoderType\": \"internal\",\r\n\
			\"address\": 5,\r\n\
			\"updateTime\": 50,\r\n\
			\"PA\": [\r\n\
				{\r\n\
					\"name\": \"PA01\",\r\n\
					\"value\": \"1000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA02\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA03\",\r\n\
					\"value\": \"0002\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA04\",\r\n\
					\"value\": \"2000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA05\",\r\n\
					\"value\": \"10000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA06\",\r\n\
					\"value\": \"1000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA07\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA08\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA09\",\r\n\
					\"value\": \"16\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA10\",\r\n\
					\"value\": \"300\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA11\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA12\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA13\",\r\n\
					\"value\": \"0112\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA14\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA15\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA16\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA17\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA18\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA19\",\r\n\
					\"value\": \"00AA\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA20\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA21\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA22\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA25\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				}\r\n\
			],\r\n\
			\"PB\": [\r\n\
				{\r\n\
					\"name\": \"PB01\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB02\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB03\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB04\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB05\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB06\",\r\n\
					\"value\": \"7.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB07\",\r\n\
					\"value\": \"15.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB08\",\r\n\
					\"value\": \"37.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB09\",\r\n\
					\"value\": \"823\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB10\",\r\n\
					\"value\": \"33.7\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB11\",\r\n\
					\"value\": \"980\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB12\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB13\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB14\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB15\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB16\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB17\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB18\",\r\n\
					\"value\": \"3141\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB19\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB20\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB21\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB22\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB25\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB27\",\r\n\
					\"value\": \"10\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB28\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB29\",\r\n\
					\"value\": \"7.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB30\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB32\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB33\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB34\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB35\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB36\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB46\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB47\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB48\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB49\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB50\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB51\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB52\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB53\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB54\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB55\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB56\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB57\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB58\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB59\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB60\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PC\": [\r\n\
				{\r\n\
					\"name\": \"PC01\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC02\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC03\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC04\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC05\",\r\n\
					\"value\": \"100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC06\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC07\",\r\n\
					\"value\": \"1000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC08\",\r\n\
					\"value\": \"200\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC09\",\r\n\
					\"value\": \"300\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC10\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC11\",\r\n\
					\"value\": \"800\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC12\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC13\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC14\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC15\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC16\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC17\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC18\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC19\",\r\n\
					\"value\": \"0010\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC22\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC24\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC27\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC30\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC32\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC33\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC34\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC35\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC36\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC37\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC38\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC39\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC40\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC43\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC44\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC51\",\r\n\
					\"value\": \"100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC54\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC60\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC66\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC67\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC68\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC69\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC70\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC71\",\r\n\
					\"value\": \"0040\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC72\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC73\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PD\": [\r\n\
				{\r\n\
					\"name\": \"PD01\",\r\n\
					\"value\": \"1C04\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD03\",\r\n\
					\"value\": \"0202\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD04\",\r\n\
					\"value\": \"0202\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD05\",\r\n\
					\"value\": \"2100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD06\",\r\n\
					\"value\": \"2021\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD07\",\r\n\
					\"value\": \"0704\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD08\",\r\n\
					\"value\": \"0707\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD09\",\r\n\
					\"value\": \"0805\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD10\",\r\n\
					\"value\": \"0808\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD11\",\r\n\
					\"value\": \"0303\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD12\",\r\n\
					\"value\": \"3803\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD13\",\r\n\
					\"value\": \"2006\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD14\",\r\n\
					\"value\": \"3920\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD17\",\r\n\
					\"value\": \"0A0A\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD18\",\r\n\
					\"value\": \"0A00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD19\",\r\n\
					\"value\": \"0B0B\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD20\",\r\n\
					\"value\": \"0B00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD21\",\r\n\
					\"value\": \"2323\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD22\",\r\n\
					\"value\": \"2B23\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD23\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD24\",\r\n\
					\"value\": \"000C\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD25\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD26\",\r\n\
					\"value\": \"0005\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD28\",\r\n\
					\"value\": \"0002\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD29\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD30\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD31\",\r\n\
					\"value\": \"0100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD32\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD33\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD34\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD41\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD42\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD43\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD44\",\r\n\
					\"value\": \"3A00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD46\",\r\n\
					\"value\": \"3B00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD47\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				}\r\n\
			],\r\n\
			\"PE\": [\r\n\
				{\r\n\
					\"name\": \"PE41\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE44\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE45\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE46\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE47\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE48\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE49\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE50\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PF\": [\r\n\
				{\r\n\
					\"name\": \"PF09\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF15\",\r\n\
					\"value\": \"2000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF18\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF21\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF23\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF25\",\r\n\
					\"value\": \"200\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF34\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF46\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			]\r\n\
		},\r\n\
		{\r\n\
			\"name\": \"srv_axis_6\",\r\n\
			\"type\": \"mitsuservo_type_A\",\r\n\
			\"encoderType\": \"internal\",\r\n\
			\"address\": 7,\r\n\
			\"updateTime\": 50,\r\n\
			\"PA\": [\r\n\
				{\r\n\
					\"name\": \"PA01\",\r\n\
					\"value\": \"1000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA02\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA03\",\r\n\
					\"value\": \"0002\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA04\",\r\n\
					\"value\": \"2000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA05\",\r\n\
					\"value\": \"10000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA06\",\r\n\
					\"value\": \"1000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA07\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA08\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA09\",\r\n\
					\"value\": \"16\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA10\",\r\n\
					\"value\": \"300\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA11\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA12\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA13\",\r\n\
					\"value\": \"0112\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA14\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA15\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA16\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA17\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA18\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA19\",\r\n\
					\"value\": \"00AA\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA20\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA21\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA22\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA25\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PA26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				}\r\n\
			],\r\n\
			\"PB\": [\r\n\
				{\r\n\
					\"name\": \"PB01\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB02\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB03\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB04\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB05\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB06\",\r\n\
					\"value\": \"7.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB07\",\r\n\
					\"value\": \"15.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB08\",\r\n\
					\"value\": \"37.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB09\",\r\n\
					\"value\": \"823\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB10\",\r\n\
					\"value\": \"33.7\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB11\",\r\n\
					\"value\": \"980\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB12\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB13\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB14\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB15\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB16\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB17\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB18\",\r\n\
					\"value\": \"3141\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB19\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB20\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB21\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB22\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB25\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB27\",\r\n\
					\"value\": \"10\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB28\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB29\",\r\n\
					\"value\": \"7.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB30\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB32\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB33\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB34\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB35\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB36\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB46\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB47\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB48\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB49\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB50\",\r\n\
					\"value\": \"4500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB51\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB52\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB53\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB54\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB55\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB56\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB57\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB58\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB59\",\r\n\
					\"value\": \"0.00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PB60\",\r\n\
					\"value\": \"0.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PC\": [\r\n\
				{\r\n\
					\"name\": \"PC01\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC02\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC03\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC04\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC05\",\r\n\
					\"value\": \"100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC06\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC07\",\r\n\
					\"value\": \"1000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC08\",\r\n\
					\"value\": \"200\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC09\",\r\n\
					\"value\": \"300\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC10\",\r\n\
					\"value\": \"500\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC11\",\r\n\
					\"value\": \"800\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC12\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC13\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC14\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC15\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC16\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC17\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC18\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC19\",\r\n\
					\"value\": \"0010\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC22\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC23\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC24\",\r\n\
					\"value\": \"0001\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC26\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC27\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC30\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC32\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC33\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC34\",\r\n\
					\"value\": \"1\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC35\",\r\n\
					\"value\": \"100.0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC36\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC37\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC38\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC39\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC40\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC43\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC44\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC51\",\r\n\
					\"value\": \"100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC54\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC60\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC66\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC67\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC68\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC69\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC70\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC71\",\r\n\
					\"value\": \"0040\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC72\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PC73\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PD\": [\r\n\
				{\r\n\
					\"name\": \"PD01\",\r\n\
					\"value\": \"1C04\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD03\",\r\n\
					\"value\": \"0202\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD04\",\r\n\
					\"value\": \"0202\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD05\",\r\n\
					\"value\": \"2100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD06\",\r\n\
					\"value\": \"2021\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD07\",\r\n\
					\"value\": \"0704\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD08\",\r\n\
					\"value\": \"0707\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD09\",\r\n\
					\"value\": \"0805\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD10\",\r\n\
					\"value\": \"0808\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD11\",\r\n\
					\"value\": \"0303\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD12\",\r\n\
					\"value\": \"3803\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD13\",\r\n\
					\"value\": \"2006\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD14\",\r\n\
					\"value\": \"3920\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD17\",\r\n\
					\"value\": \"0A0A\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD18\",\r\n\
					\"value\": \"0A00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD19\",\r\n\
					\"value\": \"0B0B\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD20\",\r\n\
					\"value\": \"0B00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD21\",\r\n\
					\"value\": \"2323\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD22\",\r\n\
					\"value\": \"2B23\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD23\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD24\",\r\n\
					\"value\": \"000C\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD25\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD26\",\r\n\
					\"value\": \"0005\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD28\",\r\n\
					\"value\": \"0002\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD29\",\r\n\
					\"value\": \"0004\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD30\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD31\",\r\n\
					\"value\": \"0100\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD32\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD33\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD34\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD41\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD42\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD43\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD44\",\r\n\
					\"value\": \"3A00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD46\",\r\n\
					\"value\": \"3B00\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PD47\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				}\r\n\
			],\r\n\
			\"PE\": [\r\n\
				{\r\n\
					\"name\": \"PE41\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE44\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE45\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE46\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE47\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE48\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE49\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PE50\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			],\r\n\
			\"PF\": [\r\n\
				{\r\n\
					\"name\": \"PF09\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF15\",\r\n\
					\"value\": \"2000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF18\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF21\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF23\",\r\n\
					\"value\": \"50\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF24\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF25\",\r\n\
					\"value\": \"200\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF31\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF34\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF45\",\r\n\
					\"value\": \"0000\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"HEX\"\r\n\
				},\r\n\
				{\r\n\
					\"name\": \"PF46\",\r\n\
					\"value\": \"0\",\r\n\
					\"default\": true,\r\n\
					\"type\": \"DEC\"\r\n\
				}\r\n\
			]\r\n\
		},\r\n\
		{\r\n\
			\"name\": \"sync_Az\",\r\n\
			\"type\": \"sync\",\r\n\
			\"address\": 0,\r\n\
			\"updateTime\": 50\r\n\
		}\r\n\
	]\r\n\
}\r\n";
