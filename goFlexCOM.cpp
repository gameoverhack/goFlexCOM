/*
 *  goFlexCOM.cpp
 *  emptyExample
 *
 *  Created by gameover on 4/06/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#include "goFlexCOM.h"

bool goFlexCOM::setup(int port) {
	ofLog(OF_LOG_VERBOSE, "Setting up server and client for TCP communication with Flex");

	successServer = false;
	successServer = tcpServer.setup(port);

	ofSetFrameRate(30);

	if (successServer) {
		waitTime = ofGetElapsedTimeMillis();
		state = kFLEXCOM_LOAD;
		return true;
	} else {
		state = kFLEXCOM_ERROR;
		return false;
	}
}

void goFlexCOM::close() {
	tcpServer.close();
}

void goFlexCOM::update() {
	if (successServer && tcpServer.getNumClients() >= 1) {

		string messageSND	= "";

		for(int i = 0; i < tcpServer.getNumClients(); i++){

			//ofLog(OF_LOG_VERBOSE, "STATE: %s", states[state].c_str());

			string messageRCV	= tcpServer.receive(i);;

			switch (state) {
				case kFLEXCOM_HAND:
					ofLog(OF_LOG_VERBOSE, "Sending kFLEXCOM_HAND kFLEXCOM_SHAKE");
					messageSND = "kFLEXCOM_HAND";
					state = kFLEXCOM_SHAKE;
					break;
				case kFLEXCOM_SHAKE:
					if (messageRCV == "kFLEXCOM_SHAKE") {
						ofLog(OF_LOG_VERBOSE, "Received kFLEXCOM_HAND kFLEXCOM_SHAKE");
						state = kFLEXCOM_READY;
					} else {
						if (ofGetElapsedTimeMillis() > waitTime + MAX_WAIT_TIME) {
							waitTime = ofGetElapsedTimeMillis();
							state = kFLEXCOM_HAND;
						}
					}

					break;
				case kFLEXCOM_READY:
					if (messageRCV != "") {
						ofLog(OF_LOG_VERBOSE, "TCPRCV: %s", messageRCV.c_str());
						ofNotifyEvent(messageReceived, messageRCV, this);
						if (messageRCV == "EXIT") {
							cout << "EXIT RCV" << endl;
							messageSND = "EXIT";
						}
						if (messageRCV == "BYE") {
							ofSleepMillis(2000);
							waitTime = ofGetElapsedTimeMillis();
							state = kFLEXCOM_LOAD;
						}
					}
					break;
				case kFLEXCOM_ERROR:
					ofLog(OF_LOG_VERBOSE, "kFLEXCOM_ERROR!!!");
					break;
				default:
					break;
			}

		}

		if (messageSND != "") {
			sendToAll(messageSND);
		}

	}

}

void goFlexCOM::sendToAll(string messageSND) {
	ofSleepMillis(200);
	int sent = tcpServer.sendToAll(messageSND.c_str());
	ofLog(OF_LOG_VERBOSE, "TCPSND: %s with %d bytes", messageSND.c_str(), sent);
}

void goFlexCOM::setState(int _state) {
	state = _state;
}

bool goFlexCOM::checkState(int _state) {
	if (_state == state) {
		return true;
	} else return false;
}

int	goFlexCOM::getState() {
	return state;
}
