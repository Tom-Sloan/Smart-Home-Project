
import UIKit
protocol saveIp {
  func setIpAddress(ip: String)
}
class AddIPAddress: UIViewController {
    var delegate: saveIp?
    var ipAdd = "Not set"
    
    @IBAction func saveBtn(_ sender: Any) {
        print("Saving")
        delegate?.setIpAddress(ip: textField.text ?? "ERROR shits fucked up")
        _ = self.navigationController?.popViewController(animated: true)
    }
    @IBOutlet var textField: UITextField!
    @IBOutlet var currentIP: UILabel!
    override func viewDidLoad() {
        super.viewDidLoad()
     
        currentIP.text = ipAdd
        // Do any additional setup after loading the view.
      
      
    }
    

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
    }
    */

}
