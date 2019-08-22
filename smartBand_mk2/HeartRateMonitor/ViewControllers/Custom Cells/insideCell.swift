

import UIKit
protocol alarmsToAlarm {
  func handleAlarmSegue(rowNumber: Int)
}
class insideCell: UITableViewCell {
  
  @IBOutlet var myView: UIView!
  @IBOutlet var toggleSwitch: UISwitch!
  @IBOutlet var timeLabel: UILabel!
  @IBOutlet var repeatDaysLabel: UILabel!
  
  var delegate : alarmsToAlarm?
  var cellIndex = 0
  
  @IBAction func toggle(_ sender: Any) {
    print(toggleSwitch.isOn)
  }
  
  override func awakeFromNib() {
    super.awakeFromNib()
    let tapGesture = UITapGestureRecognizer(target: self, action: #selector(handleTap))
    myView.addGestureRecognizer(tapGesture)
    print("OK")
    
  }
  

@objc func handleTap(){
  print("Row: \(cellIndex)")
  delegate?.handleAlarmSegue(rowNumber: cellIndex)
}
  override func setSelected(_ selected: Bool, animated: Bool) {
    super.setSelected(selected, animated: animated)
    
    // Configure the view for the selected state
  }
  
}

