//
//  ViewController.swift
//  tableView
//
//  Created by Thomas Sloan on 2019-07-04.
//  Copyright © 2019 Thomas Sloan. All rights reserved.
//cellTest_mk1

import UIKit
import Firebase

class ViewController: UIViewController, UITableViewDelegate, UITableViewDataSource, btnEvent {
  var alarms : [userAlarm] = []
  
    @IBOutlet var outerView: UIView!
    @IBOutlet var messageTableView: UITableView!
  var messageArray : [Test] = [Test]()
  var stepArr : [String] = [String]()
  let numberOfRows = 5
  var fromCellTap = false
  var row = 0
  var isEmpty = true
  override func viewWillDisappear(_ animated: Bool) {
    super.viewWillDisappear(animated)
    
    // Show the Navigation Bar
    self.navigationController?.setNavigationBarHidden(false, animated: true)
  }
  override func viewWillAppear(_ animated: Bool) {
    super.viewWillAppear(true)
    navigationController?.setNavigationBarHidden(true, animated: true)
  }
  override func viewDidLoad() {
    
    //TODO: Set yourself as the delegate and datasource here:
    messageTableView.delegate = self
    messageTableView.dataSource = self
    //messageTableView.rowHeight = 220.0
    configureTableView()
    outerView.backgroundColor = .bgColor
    messageTableView.backgroundColor = .bgColor
        
    
    messageArray = [Test.init(image: #imageLiteral(resourceName: "today_apps"), title: "Stuff", content: "80")]
    messageTableView.separatorStyle = .none
    messageTableView.register(UINib(nibName: "TableViewCell", bundle: nil), forCellReuseIdentifier: "cellTest_mk1")
    messageTableView.register(UINib(nibName: "graphCell", bundle: nil), forCellReuseIdentifier: "graphCell")
    messageTableView.register(UINib(nibName: "StepCell", bundle: nil), forCellReuseIdentifier: "stepCell")
    messageTableView.register(UINib(nibName: "topCell", bundle: nil), forCellReuseIdentifier: "topCell")
    
    
    stepArr = ["1", "2", "3"]
    
    
  }
  
  func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
    if messageTableView.tag == 1{
      return 5
    }
    return numberOfRows
  }
  
  func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
    
    
    if indexPath.row <= 0{
      let cell = tableView.dequeueReusableCell(withIdentifier: "stepCell", for: indexPath) as! StepCell
      cell.backgroundColor = .bgColor
      cell.titleLabelOne.text = "Steps"
      cell.titleLabelTwo.text = "HeartRate"
      cell.titleLabelThree.text = "Calories"
      cell.contentLabelTwo.text = "80"
      
      //cell.setUpInfo(device: messageArray[indexPath.row])
      
      cell.numberOfSteps = 5000
      
      return cell
      
    }else if indexPath.row == 1 {
      let cell = tableView.dequeueReusableCell(withIdentifier: "graphCell", for: indexPath) as! graphCell
      cell.backgroundColor = .bgColor
      cell.titleLabel.text = "Monthly Hours Slept"
      cell.view.backgroundColor = .white
      cell.titleLabel.textColor = .black
      return cell
      
    }else if indexPath.row == 2{
      let cell = tableView.dequeueReusableCell(withIdentifier: "cellTest_mk1", for: indexPath) as! TableViewCell
      cell.backgroundColor = .bgColor
      cell.title.text = "Exercise"
      cell.firstLabel.text = "Meditation"
      cell.firstImage.image = #imageLiteral(resourceName: "meditation")
      cell.infoView.backgroundColor = .pjLightBlue ?? .blue
      cell.secondLabel.text = "Weightlifting"
      cell.secondImage.image = #imageLiteral(resourceName: "weightlifting")
      cell.secondContentView.backgroundColor = .pjYellow ?? .red
      cell.thirdLabel.text = "Running"
      cell.thirdImage.image = #imageLiteral(resourceName: "runer-silhouette-running-fast")
      cell.thirdContentView.backgroundColor = .pjRed ?? .red
      cell.delegate = self
      return cell
    } else if indexPath.row == 3 {
      let cell = tableView.dequeueReusableCell(withIdentifier: "cellTest_mk1", for: indexPath) as! TableViewCell
      cell.backgroundColor = .bgColor
      cell.title.text = "Social"
      cell.firstLabel.text = "Feed"
      cell.firstImage.image = #imageLiteral(resourceName: "for_you")
      cell.infoView.backgroundColor = .pjYellow ?? .red
      cell.secondLabel.text = "Friends"
      cell.secondImage.image = #imageLiteral(resourceName: "meeting")
      cell.secondContentView.backgroundColor = .pjLightBlue ?? .red
      cell.thirdLabel.text = "Community"
      cell.thirdImage.image = #imageLiteral(resourceName: "people")
      cell.thirdContentView.backgroundColor = .pjDarkBlue ?? .red
      cell.delegate = self
      return cell
    }else {
      
      let cell = tableView.dequeueReusableCell(withIdentifier: "alarmCell", for: indexPath) as! alarmCell
      //cell.backgroundColor = .darkGray
      cell.backgroundColor = .bgColor
      cell.myView.layer.backgroundColor = UIColor.white.cgColor
      cell.myView.layer.cornerRadius = 20.0
      cell.myView.layer.shadowColor = UIColor.gray.cgColor
      cell.myView.layer.shadowOffset = CGSize(width: 0.0, height: 0.0)
      cell.myView.layer.shadowRadius = 5.0
      cell.myView.layer.shadowOpacity = 0.7
      cell.titleView.backgroundColor = .clear
      cell.tableView.layer.cornerRadius = 20.0
      cell.tableView.layer.maskedCorners = [.layerMinXMaxYCorner, .layerMaxXMaxYCorner]
      cell.alarms = self.alarms
      cell.delegate = self
      cell.tableView.reloadData()
      print("cell: \(cell.alarms)")
      return cell
    }
  }
  func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
    if indexPath.row == 0 {
      return CGFloat(220)
    }else {
      return CGFloat(250)
    }
  }
  override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
    if segue.destination is newAlarmController {
      if fromCellTap{
        let vc = segue.destination as! newAlarmController
        vc.mainView = self
        vc.days = alarms[self.row].days 
        fromCellTap = false
        vc.rownumber = self.row
        vc.empty = self.isEmpty
        
        vc.sun = alarms[self.row].days[0]
        vc.mon = alarms[self.row].days[1]
        vc.tues = alarms[self.row].days[2]
        vc.wed = alarms[self.row].days[3]
        vc.thur = alarms[self.row].days[4]
        vc.fri = alarms[self.row].days[5]
        vc.sat = alarms[self.row].days[6]
        
      }else{
        let vc = segue.destination as! newAlarmController
        vc.mainView = self
      }
    }
    
  }
    func createNewAlarm(alarm : userAlarm){
      alarms.append(alarm)
      self.isEmpty = false
      messageTableView.reloadData()
      
    }
  func modifyAlarm (alarm : userAlarm, rowNumber : Int){
    alarms[rowNumber] = alarm
    messageTableView.reloadData()
  }
    func didTapRight(boo: Bool) {
      print("FUCK YA Right", boo)
    }
    func didTapLeft(boo: Bool) {
      print("Left", boo)
    }
    //TODO: Declare configureTableView here:
    func configureTableView(){
      messageTableView.rowHeight = UITableView.automaticDimension
      messageTableView.estimatedRowHeight = 115.0
      
    }
    
}
extension ViewController : alarmCellDetect{
  
  func alarmCellTapped(rowNumber: Int) {
    fromCellTap = true
    self.row = rowNumber
    performSegue(withIdentifier: "tappedCellToEditAlarm", sender: self)
  }
  func changeAlarms(alarms : [userAlarm], isEmpty : Bool){
    self.alarms = alarms
    self.isEmpty = isEmpty
  }
  
  
}







