

import Foundation
import UIKit

class Device {
  var name : String
  var battery : Int
  var profileImageUrl : String
  var motion : String
  var motionNameON : String
  var motionNameOFF : String
  var location : String
  var id : String
  var bleId : String
  
  init(bleId : String, name : String, battery : Int, motion : String, motionChange : String, location : String, id : String, profileImageUrl :String) {
    self.name = name
    self.battery = battery
    
    self.motion = motion
    self.location = location
    self.id = id
    self.bleId = bleId
    self.profileImageUrl = profileImageUrl
    
    
    
    if motionChange == "ON"{
      motionNameON = "ON"
      motionNameOFF = "OFF"
    }else{
      motionNameOFF = "Down"
      motionNameON = "Up"
    }
  }
}
