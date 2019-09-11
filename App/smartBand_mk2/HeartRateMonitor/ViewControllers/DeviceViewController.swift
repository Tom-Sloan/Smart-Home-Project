import UIKit
import CoreBluetooth
import CoreNFC
import Firebase

let heartRateServiceCBUUID = CBUUID(string: "0x6666")
let heartRateMeasurementCharacteristicCBUUID = CBUUID(string: "2A37")
let bodySensorLocationCharacteristicCBUUID = CBUUID(string: "2A38")
let controlStatusCharacteristicCBUUID = CBUUID(string: "0x0001")
private let move = movement()

var ipAddress = "192.168.0.186"
var means: [Int] = Array(repeating: 0, count: 9)
var val = true
var setColorLeft = false
var setColorRight = false
var nfcSession: NFCNDEFReaderSession?
var dev = Device.init(bleId : "5656", name : "Name", battery : 88, motion : "Up", motionChange : "ON", location : "House", id: "NA", profileImageUrl : "")
var row = 0
var newBlueToothId = ""


var devicesList : [Device] = []
class DeviceViewController: UIViewController, deviceToDeviceSegue{
  
  
  
  @IBOutlet var tableView: UITableView!
  
  
  
  @IBAction func addDevice(_ sender: Any) {
    print("Starting NFC")
    guard nfcSession == nil else {
      print("Cancelled NFC")
      return
    }
    nfcSession = NFCNDEFReaderSession(delegate: self, queue: nil, invalidateAfterFirstRead: true)
    nfcSession?.alertMessage = "Hold your iPhone near the device to add it"
    nfcSession?.begin()
    
  }
  
  @IBOutlet var tabBarIcon: UITabBarItem!
  
  var centralManager: CBCentralManager!
  var heartRatePeripheral: CBPeripheral!
  
  override func viewDidLoad() {
    
    super.viewDidLoad()
    devicesList.removeAll()
    
    
    centralManager = CBCentralManager(delegate: self, queue: nil)
    tableView.delegate = self
    tableView.dataSource = self
    tableView.rowHeight = 100
    
    tableView.register(UINib(nibName: "deviceCell", bundle: nil), forCellReuseIdentifier: "deviceCell")
    tableView.register(UINib(nibName: "graphCell", bundle: nil), forCellReuseIdentifier: "graphCell")
    
    
    let myDataBase = Database.database().reference()
    myDataBase.child("Devices").queryOrderedByKey().observe(.childAdded, with: {
      snapshot in
      
      let orderID = snapshot.key 
      print(orderID)
      let snapshotValue = snapshot.value as? NSDictionary
      let name = snapshotValue?["Name"] as? String
      let loc = snapshotValue?["Location"] as? String
      let motion = snapshotValue?["Motion"] as? String
      let bleId = snapshotValue?["bleId"] as? String
      let url = snapshotValue?["profileImageUrl"] as? String
      var check = true
      
      for val in devicesList {
        if orderID == val.id {
          check = false
        }
      }
      if check {
        devicesList.insert(Device.init( bleId: bleId ?? "6666", name: name!, battery: 88, motion: motion ?? "Mot", motionChange: "String", location: loc!, id: orderID, profileImageUrl: url ?? "https://firebasestorage.googleapis.com/v0/b/xtech-427ca.appspot.com/o/device_images%2FE3DC41EF-3E72-44C0-8627-2A082EB5ADC5.png?alt=media&token=ccdb7e7a-3962-41d2-89af-6e6d2740715b"), at: 0)
        print("Name: \(String(describing: name)) Loc: \(String(describing: loc))")
        self.sendValue(parameters: ["action" : "create", "bleId" : bleId ?? "6666", "motionOn" : motion ?? "Mot"])
      }
      self.tableView.reloadData()
      print("LIST: \(devicesList)")
    })
    print("LIST: \(devicesList)")
    
  }
  override func viewWillDisappear(_ animated: Bool) {
    super.viewWillDisappear(animated)
    nfcSession = nil
  }
  public func dataFromFile(_ filename: String) -> Data? {
    @objc class TestClass: NSObject { }
    let bundle = Bundle(for: TestClass.self)
    if let path = bundle.path(forResource: filename, ofType: "json") {
      return (try? Data(contentsOf: URL(fileURLWithPath: path)))
    }
    return nil
  }
  
  
  func onHeartRateReceived(_ heartRate: Int) {
    //heartRateLabel.text = String(heartRate)
    //print("BPM: \(heartRate)")
  }
  
  func onControlStatusRecieved(_ status : Int){
    
    print("BLIND \(status)")
    event(value: status)
  }
}

extension DeviceViewController: CBCentralManagerDelegate {
  func centralManagerDidUpdateState(_ central: CBCentralManager) {
    switch central.state {
    case .unknown:
      print("central.state is .unknown")
    case .resetting:
      print("central.state is .resetting")
    case .unsupported:
      print("central.state is .unsupported")
    case .unauthorized:
      print("central.state is .unauthorized")
    case .poweredOff:
      print("Bluetooth Disabled- Make sure your Bluetooth is turned on")
      let alertVC = UIAlertController(title: "Bluetooth is not enabled", message: "Make sure that your bluetooth is turned on", preferredStyle: UIAlertController.Style.alert)
      let action = UIAlertAction(title: "ok", style: UIAlertAction.Style.default, handler: { (action: UIAlertAction) -> Void in
        self.dismiss(animated: true, completion: nil)
      })
      alertVC.addAction(action)
      self.present(alertVC, animated: true, completion: nil)
    case .poweredOn:
      print("central.state is .poweredOn")
      centralManager.scanForPeripherals(withServices: [heartRateServiceCBUUID])
    default:
      break
    }
  }
  
  func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral,
                      advertisementData: [String : Any], rssi RSSI: NSNumber) {
    print(peripheral)
    heartRatePeripheral = peripheral
    heartRatePeripheral.delegate = self
    centralManager.stopScan()
    centralManager.connect(heartRatePeripheral)
  }
  
  func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
    print("Connected!")
    heartRatePeripheral.discoverServices([heartRateServiceCBUUID])
  }
}

extension DeviceViewController: CBPeripheralDelegate {
  func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
    guard let services = peripheral.services else { return }
    for service in services {
      print(service)
      peripheral.discoverCharacteristics(nil, for: service)
    }
  }
  
  func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
    guard let characteristics = service.characteristics else { return }
    
    for characteristic in characteristics {
      print(characteristic)
      
      if characteristic.properties.contains(.read) {
        print("\(characteristic.uuid): properties contains .read")
        peripheral.readValue(for: characteristic)
      }
      if characteristic.properties.contains(.notify) {
        print("\(characteristic.uuid): properties contains .notify")
        peripheral.setNotifyValue(true, for: characteristic)
      }
    }
  }
  
  func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
    switch characteristic.uuid {
      //case bodySensorLocationCharacteristicCBUUID:
      //let bodySensorLocation = bodyLocation(from: characteristic)
    //bodySensorLocationLabel.text = bodySensorLocation
    case heartRateMeasurementCharacteristicCBUUID:
      let bpm = heartRate(from: characteristic)
      onHeartRateReceived(bpm)
    case controlStatusCharacteristicCBUUID:
      let stat = controlStatus(from: characteristic);
      print("Stat \(stat)")
      if stat == 1{
        let input = movementInput(acc_x_mean: Double(means[0]), acc_y_mean: Double(means[1]), acc_z_mean: Double(means[2]), gyr_x_mean: Double(means[3]), gyr_y_mean: Double(means[4]), gyr_z_mean: Double(means[5]), mag_x_mean: Double(means[6]), mag_y_mean: Double(means[7]), mag_z_mean: Double(means[8]))
        
        guard let output = try? move.prediction(input: input) else {
          return
        }
        event(value: Int(output.Movement))
      }
      
    default:
      print("Unhandled Characteristic UUID: \(characteristic.uuid)")
    }
  }
  
  private func bodyLocation(from characteristic: CBCharacteristic) -> String {
    guard let characteristicData = characteristic.value,
      let byte = characteristicData.first else { return "Error" }
    
    switch byte {
    case 0: return "Other"
    case 1: return "Chest"
    case 2: return "Wrist"
    case 3: return "Finger"
    case 4: return "Hand"
    case 5: return "Ear Lobe"
    case 6: return "Foot"
    default:
      return "Reserved for future use"
    }
  }
  
  private func heartRate(from characteristic: CBCharacteristic) -> Int {
    
    guard let characteristicData = characteristic.value else { return -1 }
    let byteArray = [UInt8](characteristicData)
    
    // See: https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.heart_rate_measurement.xml
    // The heart rate mesurement is in the 2nd, or in the 2nd and 3rd bytes, i.e. one one or in two bytes
    // The first byte of the first bit specifies the length of the heart rate data, 0 == 1 byte, 1 == 2 bytes
    let firstBitValue = byteArray[0] & 0x01
    if firstBitValue == 0 {
      // Heart Rate Value Format is in the 2nd byte
      return Int(byteArray[1])
    } else {
      // Heart Rate Value Format is in the 2nd and 3rd bytes
      return (Int(byteArray[1]) << 8) + Int(byteArray[2])
    }
  }
  private func controlStatus(from characteristic: CBCharacteristic) -> Int {
    guard let characteristicData = characteristic.value else { return -1}
    print("Shits happening")
    let byteArray = [UInt8](characteristicData)
    print(byteArray)
    print(characteristicData)
    convert(a: byteArray, round: val)
    if val {
      return 1
    }
    return 0
  }
  
  func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
    centralManager.scanForPeripherals(withServices: [heartRateServiceCBUUID])
    print("DISCONNECTED")
  }
  
  func convert(a: [UInt8], round: Bool){
    print(a)
    for  c in stride (from: 0, to: 20, by: 4){
      var temp = 0
      for i in 1...3{
        if i == 1 {
          temp += Int(a[c+i]) * 10000
          
        }else if i == 2{
          temp += Int(a[c+i]) * 100
          
        }else if i == 3{
          temp += Int(a[c+i])
          
        }
      }
      
      if a[c] == 1{
        temp *= -1
      }
      if !round{
        means[c/4] = temp
      }else  if round && c/4 + 5 < 9{
        means[c/4 + 5] = temp
      }
      print(temp)
    }
    if val {
      val = false
    }else{
      val = true
    }
    
  }
  /*
   case 0:
   parameters = ["id": "Movement", "action": "Down_To_Up"]
   case 1:
   parameters = ["id": "Movement", "action": "Left_To_Right"]
   case 2:
   parameters = ["id": "Movement", "action": "Up_To_Down"]
   case 3:
   parameters = ["id": "Movement", "action": "Right_To_Left"]
   case 4:
   parameters = ["id": "Movement", "action": "Point"]
   case 5:
   parameters = ["id": "Movement", "action": "Dial_Left_To_Right"]
   case 6:
   parameters = ["id": "Movement", "action": "Dial_Right_To_Left"]
   default:
   parameters = ["id": "", "action" : ""]
 */
  func event(value: Int){
    print("Value: ", value)
    var parameters = ["id": "", "action" : ""]
    switch value{
    case 0:
      parameters = ["id": "Movement", "action": "Down_To_Up", "bleId" : "7777"]
    case 1:
      parameters = ["id": "Movement", "action": "Left_To_Right", "bleId" : "7777"]
    case 2:
      parameters = ["id": "Movement", "action": "Up_To_Down", "bleId" : "6666"]
    case 3:
      parameters = ["id": "Movement", "action": "Right_To_Left", "bleId" : "6666"]
    case 4:
      parameters = ["id": "Movement", "action": "Point", "bleId" : "0000"]
    case 5:
      parameters = ["id": "Movement", "action": "Dial_Left_To_Right", "bleId" : "9999"]
    case 6:
      parameters = ["id": "Movement", "action": "Dial_Right_To_Left", "bleId" : "9999"]
    default:
      parameters = ["id": "", "action" : ""]
    }
    print(parameters)
    sendValue(parameters: parameters)
  }
  
  func sendValue(parameters: Dictionary<String, String>){
    
    if(parameters["id"] != "" && parameters["action"] != ""){
      guard let url = URL(string: "http://" + ipAddress) else { return }
      print("Sending to: ", ipAddress)
      var request = URLRequest(url: url)
      request.httpMethod = "POST"
      request.addValue("application/json", forHTTPHeaderField: "Content-Type")
      guard let httpBody = try? JSONSerialization.data(withJSONObject: parameters, options: []) else { return }
      request.httpBody = httpBody
      print("Sending", parameters)
      let session = URLSession.shared
      session.dataTask(with: request) { (data, response, error) in
        if let response = response {
          print(response)
        }
        
        if let data = data {
          do {
            let json = try JSONSerialization.jsonObject(with: data, options: [])
            print(json)
          } catch {
            print(error)
          }
        }
        
        }.resume()
    }
  }
}
extension DeviceViewController : UITableViewDelegate, UITableViewDataSource{
  func tableView(_ tableView: UITableView, editActionsForRowAt indexPath: IndexPath) -> [UITableViewRowAction]? {
    
    let delete = UITableViewRowAction(style: .destructive, title: "Delete") { (action, indexPath) in
      // delete item at indexPath
      let d  = devicesList[indexPath.row].id
      self.sendValue(parameters: ["action" : "delete", "bleId" : devicesList[indexPath.row].bleId ])
      devicesList.remove(at: indexPath.row)
      tableView.deleteRows(at: [indexPath], with: .fade)
      let myDataBase = Database.database().reference()
      myDataBase.child("Devices").child(d).removeValue()
      print(devicesList)
      
      
    }
    
    let share = UITableViewRowAction(style: .default, title: "Share") { (action, indexPath) in
      // share item at indexPath
      print("I want to share: \(devicesList[indexPath.row])")
    }
    
    share.backgroundColor = UIColor.lightGray
    
    return [delete, share]
    
  }
  func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
    return devicesList.count
  }
  
  func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
    
    
    
    let cell = tableView.dequeueReusableCell(withIdentifier: "deviceCell", for: indexPath) as! deviceCell
    
    cell.contentsName.text = devicesList[indexPath.row].name
    
    cell.contentsBattery.text = "88%"
    
    cell.contentsMotion.text = devicesList[indexPath.row].location
    
    cell.delegate = self
    cell.ipAddress = ipAddress
    print(ipAddress)
    cell.row = indexPath.row
    
    
    return cell
    
    
  }
  func handleDeviceSegue(rowNumber: Int){
    
    print("Seg time! Row Number: \(rowNumber)")
    row = rowNumber
    performSegue(withIdentifier: "devicesToDevice", sender: self)
  }
  
  override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
    
    if segue.destination is DeviceInfromationViewController {
      let vc = segue.destination as! DeviceInfromationViewController
      
      vc.device = devicesList[row]
    }
    else if segue.destination is newDeviceViewController {
      let vc = segue.destination as! newDeviceViewController
      vc.bleId = newBlueToothId
      print("Setting vc.bleId: ", newBlueToothId)
    }else if segue.destination is AddIPAddress {
      let vc = segue.destination as! AddIPAddress
      vc.ipAdd = ipAddress
      vc.delegate = self
      print("Setting ip as  ", ipAddress)
    }
    
  }
  
}
extension DeviceViewController: NFCNDEFReaderSessionDelegate{
  
  //  To scan:
  //  nfcSession = NFCNDEFReaderSession.init(delegate: self, queue: nil, invalidateAfterFirstRead: true)
  //  nfcSession?.begin()
  func readerSession(_ session: NFCNDEFReaderSession, didInvalidateWithError error: Error) {
    print("NFC Ready: \(nfcSession?.isReady ?? false)")
    // Check the invalidation reason from the returned error.
    if let readerError = error as? NFCReaderError {
      print(error)
      // Show an alert when the invalidation reason is not because of a success read
      // during a single tag read mode, or user canceled a multi-tag read mode session
      // from the UI or programmatically using the invalidate method call.
      if (readerError.code != .readerSessionInvalidationErrorFirstNDEFTagRead)
        && (readerError.code != .readerSessionInvalidationErrorUserCanceled) {
        let alertController = UIAlertController(
          title: "Session Invalidated",
          message: error.localizedDescription,
          preferredStyle: .alert
        )
        alertController.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
        DispatchQueue.main.async {
          self.present(alertController, animated: true, completion: nil)
        }
      }
    }
    
    // A new session instance is required to read new tags.
    nfcSession = nil
  }
  
  func readerSession(_ session: NFCNDEFReaderSession, didDetectNDEFs messages: [NFCNDEFMessage]) {
  
    var result = ""
    for payload in messages[0].records {
      result += String.init(data: payload.payload.advanced(by: 3), encoding: .utf8)! // 1
    }
    
    // If want to display
    DispatchQueue.main.async {
      newBlueToothId = result
      self.performSegue(withIdentifier: "goToNewDevice", sender: self)
    }
    nfcSession = nil
    
  }
}
extension DeviceViewController : saveIp {
  func setIpAddress(ip: String) {
    ipAddress = ip
    print(ipAddress)
    self.tableView.reloadData()
  }

  
}



