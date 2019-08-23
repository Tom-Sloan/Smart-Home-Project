
import UIKit
import Foundation

class userAlarm{
  var time : String
  var days : [Bool]
  var isActive : Bool
  
  init(time: String, days : [Bool], isActive : Bool) {
    self.time = time
    self.days = days
    self.isActive = isActive
  }
}
