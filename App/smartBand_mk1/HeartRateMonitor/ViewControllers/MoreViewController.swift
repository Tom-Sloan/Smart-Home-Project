

import UIKit
import Firebase
class MoreViewController: UIViewController, UITableViewDelegate, UITableViewDataSource, segueFunc {
  
  @IBOutlet var tableView: UITableView!
  
  override func viewDidLoad() {
    super.viewDidLoad()
    
    tableView.delegate = self
    tableView.dataSource = self
    tableView.register(UINib(nibName: "menuCell", bundle: nil), forCellReuseIdentifier: "menuCell")
  }
  
  func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
    return 3
  }
  
  func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
    if indexPath.row <= 0{
      let cell = tableView.dequeueReusableCell(withIdentifier: "menuCell", for: indexPath) as! menuCell
      cell.iconView.image = #imageLiteral(resourceName: "layers")
      cell.titleLabel.text = "FAQ"
      cell.delagte = self
      return cell
      
    }else  if indexPath.row == 1{
      let cell = tableView.dequeueReusableCell(withIdentifier: "menuCell", for: indexPath) as! menuCell
      cell.iconView.image = #imageLiteral(resourceName: "today_apps")
      cell.titleLabel.text = "About"
      cell.delagte = self
      return cell
      
    }else{
      let cell = tableView.dequeueReusableCell(withIdentifier: "menuCell", for: indexPath) as! menuCell
      cell.iconView.image = #imageLiteral(resourceName: "moon_symbol")
      cell.titleLabel.text = "Logout"
      cell.delagte = self
      return cell
    }
  }
  
  func handleSegue(location : String){
    
    print("Clicked")
    
    if location == "Logout"{
      do {
        try Auth.auth().signOut()
      }
      catch let signOutError as NSError {
        print ("Error signing out: %@", signOutError)
      }
      
      let storyboard = UIStoryboard(name: "Main", bundle: nil)
      let initial = storyboard.instantiateInitialViewController()
      UIApplication.shared.keyWindow?.rootViewController = initial
      
    }else{
      
      performSegue(withIdentifier: "menuTo\(location)", sender: self)
      
    }
  }
}



