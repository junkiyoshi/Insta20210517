#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();
	ofSetCircleResolution(72);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(ofGetFrameNum() * 0.37);
	ofRotateY(ofGetFrameNum() * 0.72);

	int radius = 300;
	int base_radius = 20;
	int number_of_satellite = 3;
	int number_of_log = 20;

	for (int param = 0; param < 360; param += 15){

		auto base_location = glm::vec3(radius * cos(param * DEG_TO_RAD), radius * sin(param * DEG_TO_RAD), 0);

		auto base_noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
		auto base_rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(base_noise_seed.x, (ofGetFrameNum() + number_of_log) * 0.0035), 0, 1, -PI, PI), glm::vec3(1, 0, 0));
		auto base_rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(base_noise_seed.y, (ofGetFrameNum() + number_of_log) * 0.0035), 0, 1, -PI, PI), glm::vec3(0, 1, 0));
		auto base_rotation_z = glm::rotate(glm::mat4(), ofMap(ofNoise(base_noise_seed.z, (ofGetFrameNum() + number_of_log) * 0.0035), 0, 1, -PI, PI), glm::vec3(0, 0, 1));
		base_location = glm::vec4(base_location, 0) * base_rotation_z * base_rotation_y * base_rotation_x;

		ofFill();
		ofSetColor(39);
		ofDrawSphere(base_location, base_radius * 0.5);

		for (int i = 0; i < number_of_satellite; i++) {

			float noise_seed_x = ofRandom(1000);
			float noise_seed_y = ofRandom(1000);
			auto location = glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
			location = glm::normalize(location) * base_radius * 1.5;
			auto prev_location = glm::vec3();

			for (int k = 0; k < number_of_log; k++) {

				auto rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed_y, (ofGetFrameNum() + k) * 0.01), 0, 1, -PI * 2, PI * 2), glm::vec3(0, 1, 0));
				auto rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed_x, (ofGetFrameNum() + k) * 0.01), 0, 1, -PI * 2, PI * 2), glm::vec3(1, 0, 0));
			
				auto tmp_base_location = glm::vec3(radius * cos(param * DEG_TO_RAD), radius * sin(param * DEG_TO_RAD), 0);
				auto tmp_rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(base_noise_seed.x, (ofGetFrameNum() + k) * 0.0035), 0, 1, -PI, PI), glm::vec3(1, 0, 0));
				auto tmp_rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(base_noise_seed.y, (ofGetFrameNum() + k) * 0.0035), 0, 1, -PI, PI), glm::vec3(0, 1, 0));
				auto tmp_rotation_z = glm::rotate(glm::mat4(), ofMap(ofNoise(base_noise_seed.z, (ofGetFrameNum() + k) * 0.0035), 0, 1, -PI, PI), glm::vec3(0, 0, 1));
				tmp_base_location = glm::vec4(tmp_base_location, 0) * tmp_rotation_z * tmp_rotation_y * tmp_rotation_x;

				auto draw_location = tmp_base_location + glm::vec4(location, 0) * rotation_y * rotation_x;

				if (k > 0) {

					ofSetColor(ofColor(39, ofMap(k, 0, number_of_log, 32, 255)));
					ofDrawLine(prev_location, draw_location);
				}

				prev_location = draw_location;
			}

			ofDrawSphere(prev_location, base_radius * 0.2);
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}