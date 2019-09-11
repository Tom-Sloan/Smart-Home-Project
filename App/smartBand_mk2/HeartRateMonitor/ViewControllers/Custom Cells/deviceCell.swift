
import UIKit
import SwiftMQTT
protocol deviceToDeviceSegue {
  func handleDeviceSegue(rowNumber: Int)
}

class deviceCell: UITableViewCell {
  @IBOutlet var contentsName: UILabel!
  @IBOutlet var contentsMotion: UILabel!
  @IBOutlet var contentsBattery: UILabel!
  @IBOutlet var stackView: UIStackView!
  var delegate : deviceToDeviceSegue?
  var row = 0
  var ipAddress = "192.168.0.187"
  var mqttSession: MQTTSession!
  
  override func awakeFromNib() {
    super.awakeFromNib()
    // Initialization code
    //contentsMotion.adjustsFontSizeToFitWidth = true
    contentsName.adjustsFontSizeToFitWidth = true
    contentsName.minimumScaleFactor = 3
    //contentsBattery.adjustsFontSizeToFitWidth = true
    
    let tapGesture = UITapGestureRecognizer(target: self, action: #selector(handleTap))
    stackView.addGestureRecognizer(tapGesture)
    print("OK")
    
    establishConnection()
    
  }
  
  @objc func handleTap(){
    print("Row: \(row)")
    
    var temp = ["action" : "up", "bleId" : "1234", "id" : "Movement"]
    let message = "1"
    let channel = "info"
    let data = message.data(using: .utf8)!
    mqttSession.publish(data, in: channel, delivering: .atMostOnce, retain: false) { (error) in
      switch error {
      case .none:
        print("Published \(message) on channel \(channel)")
      default:
        print("Error Occurred During Publish:")
        print(error.description)
      }
    }
    delegate?.handleDeviceSegue(rowNumber: row)
  }
  
  override func setSelected(_ selected: Bool, animated: Bool) {
    super.setSelected(selected, animated: animated)
    print("Tapped")
  }
  func establishConnection() {
    let host = ipAddress
    let port: UInt16 = 1883
    let clientID = self.clientID()
    
    mqttSession = MQTTSession(host: host, port: port, clientID: clientID, cleanSession: true, keepAlive: 15, useSSL: false)
    
    print("Trying to connect to \(host) on port \(port) for clientID \(clientID)")
    
    mqttSession.connect { (error) in
      if error == .none {
        print("Connected.")
        self.subscribeToChannel()
      } else {
        print("Error occurred during connection:")
        print(error.description)
      }
    }
  }
  
  func subscribeToChannel() {
    let channel = "other"
    mqttSession.subscribe(to: channel, delivering: .atLeastOnce) { (error) in
      if error == .none {
        print("Subscribed to \(channel)")
      } else {
        print("Error occurred during subscription:")
        print(error.description)
      }
    }
  }
  

  
  // MARK: - MQTTSessionDelegates
  
  func mqttDidReceive(message: MQTTMessage, from session: MQTTSession) {
    print("data received on topic \(message.topic) message \(message.stringRepresentation ?? "<>")")
  }
  
  func mqttDidDisconnect(session: MQTTSession, error: MQTTSessionError) {
    print("Session Disconnected.")
    if error != .none {
      print(error.description)
    }
  }
  
  func mqttDidAcknowledgePing(from session: MQTTSession) {
    print("Keep-alive ping acknowledged.")
  }
  
  // MARK: - Utilities
  
  func clientID() -> String {
    
    let userDefaults = UserDefaults.standard
    let clientIDPersistenceKey = "clientID"
    let clientID: String
    
    if let savedClientID = userDefaults.object(forKey: clientIDPersistenceKey) as? String {
      clientID = savedClientID
    } else {
      clientID = randomStringWithLength(5)
      userDefaults.set(clientID, forKey: clientIDPersistenceKey)
      userDefaults.synchronize()
    }
    
    return clientID
  }
  
  // http://stackoverflow.com/questions/26845307/generate-random-alphanumeric-string-in-swift
  func randomStringWithLength(_ len: Int) -> String {
    let letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
    
    var randomString = String()
    for _ in 0..<len {
      let length = UInt32(letters.count)
      let rand = arc4random_uniform(length)
      let index = String.Index(encodedOffset: Int(rand))
      randomString += String(letters[index])
    }
    return String(randomString)
  }
  
}

