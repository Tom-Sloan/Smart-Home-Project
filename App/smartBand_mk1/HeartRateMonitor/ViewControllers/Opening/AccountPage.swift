

import UIKit
import Firebase
import GoogleSignIn
import FBSDKLoginKit

class AccountPage: UIViewController, GIDSignInUIDelegate {
    
    @IBOutlet var login: UIButton!
    @IBOutlet var createAccount: UIButton!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        setUpGoogleButton()
        // Do any additional setup after loading the view.
        buttonRounding(view: login)
        buttonRounding(view: createAccount)
        
    }
    
    func buttonRounding(view : UIView){
        view.layer.cornerRadius = 20.0
        view.layer.shadowColor = UIColor.gray.cgColor
        view.layer.shadowOffset = CGSize(width: 0.0, height: 0.0)
        view.layer.shadowRadius = 5.0
        view.layer.shadowOpacity = 0.7
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(true)
        navigationController?.setNavigationBarHidden(true, animated: true)
        //autoLogin
            if Auth.auth().currentUser != nil {
              self.performSegue(withIdentifier: "gotToHome", sender: nil)
            }
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(true)
        navigationController?.setNavigationBarHidden(false, animated: false)
    }
    fileprivate func setUpGoogleButton(){
        //Google Signin Btn
        let googleBtn = GIDSignInButton()
        googleBtn.frame = CGRect(x: 16, y: 500, width: view.frame.width-32, height: 50)
        view.addSubview(googleBtn)
        
        GIDSignIn.sharedInstance().uiDelegate = self
    }
    
}


