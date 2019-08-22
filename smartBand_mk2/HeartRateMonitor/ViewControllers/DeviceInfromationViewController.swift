

import UIKit

class DeviceInfromationViewController: UIViewController {

    @IBOutlet var superView: UIView!
    @IBOutlet var titleView: UIView!
    @IBOutlet var bodyView: UIView!
    @IBOutlet var titleLabel: UILabel!
    @IBOutlet var battery: UILabel!
    @IBOutlet var imageView: UIImageView!
    @IBOutlet var dateLastUsedLabel: UILabel!
    @IBOutlet var motionLabel: UILabel!
    @IBOutlet var dateLabel: UILabel!
    @IBOutlet var pickerView: UIPickerView!
    @IBOutlet var label_one: UILabel!
    @IBOutlet var label_two: UILabel!
  
  var device : Device = Device.init(bleId : "5656", name : "Name", battery : 88, motion : "Up", motionChange : "ON", location : "House", id: "NA", profileImageUrl : "")

  override func viewDidLoad() {
    super.viewDidLoad()
    self.navigationController?.navigationBar.prefersLargeTitles = false
    battery.text = device.motion
    titleLabel.text = device.name
    motionLabel.text = device.location
    dateLabel.text = device.bleId
    //imageView.image = device.image
    print(device.profileImageUrl)
    imageView.loadImageUsingCacheWithUrlString(device.profileImageUrl)
    
  }
  
//  func setDevice (device : Device){
//    print(device.name)
//    print(device.battery)
//    var a = device.name
//    print(a)
//    titleLabel.text = a
//  }
//

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
    }
    */

}
