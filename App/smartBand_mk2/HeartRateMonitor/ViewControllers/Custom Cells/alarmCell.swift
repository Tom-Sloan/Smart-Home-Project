
import UIKit

protocol alarmCellDetect {
  func alarmCellTapped(rowNumber : Int)
  func changeAlarms(alarms : [userAlarm], isEmpty: Bool)
}
class alarmCell: UITableViewCell, UITableViewDataSource, UITableViewDelegate{
  var alarms : [userAlarm] = []
  @IBOutlet var myView: UIView!
  @IBOutlet var tableView: UITableView!
  @IBOutlet var titleView: UIView!
  @IBOutlet var titleLabel: UILabel!
  @IBOutlet var addButton: [UIButton]!
  
  var delegate : alarmCellDetect?
    
    @IBAction func btn(_ sender: Any) {
      
  
    }
    
  func tableView(_ tableView: UITableView, editActionsForRowAt indexPath: IndexPath) -> [UITableViewRowAction]? {
    
    let delete = UITableViewRowAction(style: .destructive, title: "Delete") { (action, indexPath) in
      // delete item at indexPath
      
      self.alarms.remove(at: indexPath.row)
      
      self.delegate!.changeAlarms(alarms : self.alarms, isEmpty: self.alarms.isEmpty)
      tableView.deleteRows(at: [indexPath], with: .fade)
      
      print(self.alarms)
      tableView.reloadData()
      
    }
    
    let share = UITableViewRowAction(style: .default, title: "Share") { (action, indexPath) in
      // share item at indexPath
      print("I want to share: \(devicesList[indexPath.row])")
    }
    
    share.backgroundColor = UIColor.lightGray
    
    return [delete, share]
    
  }
  func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
    return alarms.count
    
  }
  
  func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
    let cell = tableView.dequeueReusableCell(withIdentifier: "insideCell", for: indexPath) as! insideCell
    cell.timeLabel.text = alarms[indexPath.row].time
    cell.repeatDaysLabel.text = makeRepeatDaysString(days: alarms[indexPath.row].days)
    cell.cellIndex = indexPath.row
    
    //cell.doesExist = alarms.count
    cell.delegate = self
    return cell
  }
  func makeRepeatDaysString(days : [Bool]) -> String{
    var temp = ""
    if days[0]{
      temp += "Sun"
    }
    if days[1]{
      if temp != ""{
        temp += ", "
      }
      temp += "Mon"
    }
    if days[2]{
      if temp != ""{
        temp += ", "
      }
      temp += "Tues"
    }
    if days[3]{
      if temp != ""{
        temp += ", "
      }
      temp += "Wed"
    }
    if days[4]{
      if temp != ""{
        temp += ", "
      }
      temp += "Thurs"
    }
    if days[5]{
      if temp != ""{
        temp += ", "
      }
      temp += "Fri"
    }
    if days[6]{
      if temp != ""{
        temp += ", "
      }
      temp += "Sat"
    }
    return temp
  }
  
  override func awakeFromNib() {
    super.awakeFromNib()
    tableView.delegate = self
    tableView.dataSource = self
    tableView.rowHeight = 50
    // Initialization code
    tableView.register(UINib(nibName: "insideCell", bundle: nil), forCellReuseIdentifier: "insideCell")
    
  }
  
  override func setSelected(_ selected: Bool, animated: Bool) {
    super.setSelected(selected, animated: animated)
    
    // Configure the view for the selected state
  }
  
}
extension alarmCell : alarmsToAlarm{
  func handleAlarmSegue(rowNumber: Int) {
    print("\(rowNumber)")
    delegate?.alarmCellTapped(rowNumber: rowNumber)
  }
}


