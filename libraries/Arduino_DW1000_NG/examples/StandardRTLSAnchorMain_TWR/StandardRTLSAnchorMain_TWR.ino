/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/* 
 * StandardRTLSAnchorMain_TWR.ino
 * 
 * This is an example master anchor in a RTLS using two way ranging ISO/IEC 24730-62_2013 messages
 */

#include <DW1000Ng.hpp>
#include <DW1000NgUtils.hpp>
#include <DW1000NgRanging.hpp>
#include <DW1000NgRTLS.hpp>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET     4
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

typedef struct Position {
    double x;
    double y;
} Position;

// connection pins
const uint8_t PIN_SS = PB12; // spi select pin
const uint8_t PIN_RST = PC4;

// Extended Unique Identifier register. 64-bit device identifier. Register file: 0x01
char EUI[] = "AA:BB:CC:DD:EE:FF:00:01";

Position position_self = {0,0};
Position position_B = {3,0};
Position position_C = {3,2.5};

double range_self;
double range_B;
double range_C;

boolean received_B = false;

byte target_eui[8];
byte tag_shortAddress[] = {0x05, 0x00};

byte anchor_b[] = {0x02, 0x00};
uint16_t next_anchor = 2;
byte anchor_c[] = {0x03, 0x00};

byte tx_power_conf[] = {0x1F, 0x1F, 0x1F, 0x1F};

device_configuration_t DEFAULT_CONFIG = {
    false,
    true,
    true,
    true,
    false,
    SFDMode::STANDARD_SFD,
    Channel::CHANNEL_2,
    DataRate::RATE_850KBPS,
    PulseFrequency::FREQ_16MHZ,
    PreambleLength::LEN_256,
    PreambleCode::CODE_3
};

frame_filtering_configuration_t ANCHOR_FRAME_FILTER_CONFIG = {
    false,
    false,
    true,
    false,
    false,
    false,
    false,
    true /* This allows blink frames */
};

void setup() {
    // DEBUG monitoring
    Serial.begin(115200);
    Serial.println(F("### DW1000Ng-arduino-ranging-anchorMain ###"));
    
    // initialize the OLED display
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }
    else
      Serial.println(F("SSD1306 initialized ..."));
      
    // initialize the driver
    DW1000Ng::initializeNoInterrupt(PIN_SS, PIN_RST);
    Serial.println(F("DW1000Ng initialized ..."));
    
    // general configuration
    DW1000Ng::applyConfiguration(DEFAULT_CONFIG);
    DW1000Ng::setTXPower(tx_power_conf);
    DW1000Ng::enableFrameFiltering(ANCHOR_FRAME_FILTER_CONFIG);
    DW1000Ng::setEUI(EUI);
    DW1000Ng::setPreambleDetectionTimeout(64);
    DW1000Ng::setSfdDetectionTimeout(273);
    DW1000Ng::setReceiveFrameWaitTimeoutPeriod(5000);
    DW1000Ng::setNetworkId(RTLS_APP_ID);
    DW1000Ng::setDeviceAddress(1);
    DW1000Ng::setAntennaDelay(16436);
    Serial.println(F("Committed configuration ..."));
    
    // DEBUG chip info and registers pretty printed
    char msg[128];
    DW1000Ng::getPrintableDeviceIdentifier(msg);
    Serial.print("Device ID: "); Serial.println(msg);
    DW1000Ng::getPrintableExtendedUniqueIdentifier(msg);
    Serial.print("Unique ID: "); Serial.println(msg);
    DW1000Ng::getPrintableNetworkIdAndShortAddress(msg);
    Serial.print("Network ID & Device Address: "); Serial.println(msg);
    DW1000Ng::getPrintableDeviceMode(msg);
    Serial.print("Device mode: "); Serial.println(msg);    
}

/* using https://math.stackexchange.com/questions/884807/find-x-location-using-3-known-x-y-location-using-trilateration */
void calculatePosition(double &x, double &y) {

    /* This gives for granted that the z plane is the same for anchor and tags */
    double A = ( (-2*position_self.x) + (2*position_B.x) );
    double B = ( (-2*position_self.y) + (2*position_B.y) );
    double C = (range_self*range_self) - (range_B*range_B) - (position_self.x*position_self.x) + (position_B.x*position_B.x) - (position_self.y*position_self.y) + (position_B.y*position_B.y);
    double D = ( (-2*position_B.x) + (2*position_C.x) );
    double E = ( (-2*position_B.y) + (2*position_C.y) );
    double F = (range_B*range_B) - (range_C*range_C) - (position_B.x*position_B.x) + (position_C.x*position_C.x) - (position_B.y*position_B.y) + (position_C.y*position_C.y);

    x = (C*E-F*B) / (E*A-B*D);
    y = (C*D-A*F) / (B*D-A*E);
}

void loop() {
    if(DW1000NgRTLS::receiveFrame()) {
        size_t recv_len = DW1000Ng::getReceivedDataLength();
        byte recv_data[recv_len];
        DW1000Ng::getReceivedData(recv_data, recv_len);

        if(recv_data[0] == BLINK) {
            DW1000NgRTLS::transmitRangingInitiation(&recv_data[2], tag_shortAddress);
            DW1000NgRTLS::waitForTransmission();

            RangeAcceptResult result = DW1000NgRTLS::anchorRangeAccept(NextActivity::RANGING_CONFIRM, next_anchor);
            if(!result.success) {
                //Serial.println("Blink failed");
                display.clearDisplay();
                display.display();
                return;
            }
            range_self = result.range;

            String rangeString = "Range: "; rangeString += range_self; rangeString += " m";
            rangeString += "\t RX power: "; rangeString += DW1000Ng::getReceivePower(); rangeString += " dBm";
            Serial.println(rangeString);
            
//            String str = "d="; str += range_self; str += '\n'; str += "p="; str += DW1000Ng::getReceivePower();
//            Serial.println(str);

            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0,0);
            String line1 = "Range: "; line1 += range_self; line1 += " m";
            display.println(line1);
            String line2 = "RX power: "; line2 += DW1000Ng::getReceivePower(); line2 += " dBm";
            display.println(line2);
            display.display();
        }
        else if(recv_data[9] == 0x60) {
            double range = static_cast<double>(DW1000NgUtils::bytesAsValue(&recv_data[10],2) / 1000.0);
            String rangeReportString = "Range from: "; rangeReportString += recv_data[7]; rangeReportString += " = "; rangeReportString += range;
            Serial.println(rangeReportString);
            if(received_B == false && recv_data[7] == anchor_b[0] && recv_data[8] == anchor_b[1]) {
                range_B = range;
                received_B = true;
//                String b_str = "b="; b_str += range_B;
//                Serial.println(b_str);
            } else if(received_B == true && recv_data[7] == anchor_c[0] && recv_data[8] == anchor_c[1]) {
                range_C = range;
//                String c_str = "c="; c_str += range_C;
//                Serial.println(c_str);
                
                double x, y;
                calculatePosition(x, y);
                String positioning = "Found position - x: ";
                positioning += x; positioning +=" y: ";
                positioning += y;
                Serial.println(positioning);

//                String x_str = "x="; x_str += x; x_str += '\n'; x_str = "y="; x_str += y;
//                Serial.println(x_str);

                display.clearDisplay();
                display.setTextSize(1);
                display.setTextColor(SSD1306_WHITE);
                display.setCursor(0,0);
                String line1 = "X: "; line1 += x; line1 += " m";
                display.println(line1);
                String line2 = "Y: "; line2 += y; line2 += " m";
                display.println(line2);
                display.display();
                received_B = false;
            }
            else {
                received_B = false;
            }
        }
    }
}
