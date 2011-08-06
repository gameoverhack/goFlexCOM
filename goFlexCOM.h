/*
 *  goFlexCOM.h
 *  emptyExample
 *
 *  Created by gameover on 4/06/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#ifndef __H_GOFLEXCOM
#define __H_GOFLEXCOM

#include "ofMain.h"
#include "ofxNetwork.h"

#define MAX_WAIT_TIME 1000

enum states {
	HAND,
	SHAKE,
	READY,
	ERROR,
	LOAD
};

const string states[] =
{
    "HAND",
	"SHAKE",
    "READY",
    "ERROR",
	"LOAD"
};

class goFlexCOM {
	
public:
	
	bool setup(int port=6666);
	void update();
	void close();
	
	void			sendToAll(string messageSND);
	
	ofEvent<string>	messageReceived;
	
	void			setState(int _state);
	bool			checkState(int _state);
	int				getState();
	
private:
	
	int				state;
	
	ofxTCPServer	tcpServer;
	
	bool			successServer;
	
	int				waitTime;

};

#endif