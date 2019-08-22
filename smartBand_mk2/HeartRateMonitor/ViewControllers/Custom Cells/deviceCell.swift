
import UIKit

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
    
  }
  
  @objc func handleTap(){
    print("Row: \(row)")
    delegate?.handleDeviceSegue(rowNumber: row)
    var temp = ["action" : "up", "bleId" : "1234", "id" : "Movement"]
    sendValue(parameters: temp)
  }
  
  override func setSelected(_ selected: Bool, animated: Bool) {
    super.setSelected(selected, animated: animated)
    print("Tapped")
  }
  func sendValue(parameters: Dictionary<String, String>){
    
    if(parameters["id"] != "" && parameters["action"] != ""){
      guard let url = URL(string: "http://192.168.0.186") else { return }
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

