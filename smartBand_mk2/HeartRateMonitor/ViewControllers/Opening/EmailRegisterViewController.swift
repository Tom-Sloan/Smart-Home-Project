

import UIKit
import Firebase
class EmailRegisterViewController: UIViewController {

    
  @IBOutlet var email: UITextField!
  @IBOutlet var password: UITextField!
  @IBOutlet var passwordConfirm: UITextField!
  @IBOutlet var registerButton: UIButton!
  override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
    }
    
    @IBAction func registerPressed(_ sender: Any) {
      if password.text != passwordConfirm.text {
        let alertController = UIAlertController(title: "Password Incorrect", message: "Please re-type password", preferredStyle: .alert)
        let defaultAction = UIAlertAction(title: "OK", style: .cancel, handler: nil)
        
        alertController.addAction(defaultAction)
        self.present(alertController, animated: true, completion: nil)
      }
      else{
        Auth.auth().createUser(withEmail: email.text!, password: password.text!){ (user, error) in
          if error == nil {
            self.performSegue(withIdentifier: "gotToHomeFromEmailSignUp", sender: self)
          }
          else{
            let alertController = UIAlertController(title: "Error", message: error?.localizedDescription, preferredStyle: .alert)
            let defaultAction = UIAlertAction(title: "OK", style: .cancel, handler: nil)
            
            alertController.addAction(defaultAction)
            self.present(alertController, animated: true, completion: nil)
          }
        }
      }
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
