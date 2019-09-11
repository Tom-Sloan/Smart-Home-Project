

import UIKit

//protocol NewAlarmPro {
//  func createNewAlarm(alarm : userAlarm)
//}

class newAlarmController: UIViewController {
    @IBOutlet var sunday: UIButton!
    
    @IBAction func pciker(_ sender: Any) {
    
    }
  var empty = true
    @IBOutlet var monday: UIButton!
    @IBOutlet var tuesday: UIButton!
    @IBOutlet var datePicker: UIDatePicker!
    @IBOutlet var thursday: UIButton!
    @IBOutlet var wednesday: UIButton!
    @IBOutlet var saturday: UIButton!
    @IBOutlet var repeatLabel: UILabel!
    @IBOutlet var friday: UIButton!
    var rownumber = 0
  
    var mon = false, tues = false, wed = false, thur = false, fri = false, sat = false, sun = false
    var days : [Bool] = []
  
  @IBAction func sundayButton(_ sender: Any) {
    sun.toggle()
    setColor(day: sun, btn: sunday)
    
  }
  func setColor (day : Bool, btn : UIButton){
    if day {
      btn.setTitleColor(self.view.tintColor, for: .normal)
    }else{
      btn.setTitleColor(.lightGray, for: .normal)
    }
  }
  @IBAction func mondayButton(_ sender: Any) {
    mon.toggle()
    setColor(day: mon, btn: monday)
  }
  @IBAction func tuesdayButton(_ sender: Any) {
    tues.toggle()
    setColor(day: tues, btn: tuesday)
  }
  @IBAction func wednesdayButton(_ sender: Any) {
    wed.toggle()
    setColor(day: wed, btn: wednesday)
  }
  @IBAction func thursdayButton(_ sender: Any) {
    thur.toggle()
    setColor(day: thur, btn: thursday)
  }
  @IBAction func fridayButton(_ sender: Any) {
    fri.toggle()
    setColor(day: fri, btn: friday)
  }
  @IBAction func saturdayButton(_ sender: Any) {
    sat.toggle()
    setColor(day: sat, btn: saturday)
  }
  
  var mainView : ViewController?
  @IBAction func saveBtn(_ sender: Any) {
    days = [sun, mon, tues, wed, thur, fri, sat]
    
    let calendar = datePicker.calendar.dateComponents([.hour, .minute], from: datePicker.date)
    var temp = ""
    if calendar.minute! < 10 {
      temp =  String(calendar.hour!) + ":0" + String(calendar.minute!)
    }else{
      temp =  String(calendar.hour!) + ":" + String(calendar.minute!)
    }
    
    print("hour: \(String(describing: calendar.hour)), min \(String(describing: calendar.minute))")
    
    let alarm = userAlarm.init(time: temp, days:days, isActive: true)
    if self.empty {
      mainView?.createNewAlarm(alarm : alarm)
    }else{
      mainView?.modifyAlarm(alarm : alarm, rowNumber : rownumber)
      
    }
    _ = self.navigationController?.popViewController(animated: true)
  }
  
  override func viewDidLoad() {
    super.viewDidLoad()
    setColor(day: sun, btn: sunday)
    setColor(day: mon, btn: monday)
    setColor(day: tues, btn: tuesday)
    setColor(day: wed, btn: wednesday)
    setColor(day: thur, btn: thursday)
    setColor(day: fri, btn: friday)
    setColor(day: sat, btn: saturday)
    
    
    
    
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

