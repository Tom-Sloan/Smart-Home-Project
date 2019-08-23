

import UIKit
import Firebase



class newDeviceViewController: UIViewController, UINavigationControllerDelegate, UIImagePickerControllerDelegate {
  
  @IBOutlet var img: UIImageView!
  @IBOutlet var btn: UIBarButtonItem!
  
  @IBOutlet var pickerOne: UIPickerView!
  @IBOutlet var pickerTwo: UIPickerView!
  
  @IBOutlet var textFieldOne: UITextField!
  @IBOutlet var textFieldTwo: UITextField!
  
  @IBOutlet var labelOne: UILabel!
  @IBOutlet var labelTwo: UILabel!
  
  
  @IBAction func saveInfo(_ sender: Any) {
    addNewDevice()
    btn.isEnabled = false
    _ = self.navigationController?.popViewController(animated: true)
    //performSegue(withIdentifier: "newDeviceToDevices", sender: self)
  }
  var bleId = "66"
  var rowSelected = ""
  private let dataSource = ["On", "Off"]
  
  override func viewDidLoad() {
    super.viewDidLoad()
    self.navigationController?.navigationBar.prefersLargeTitles = false
    
    //img..cornerRadius = img.frame.width / 2
    // Do any additional setup after loading the view.
    
    img.image = #imageLiteral(resourceName: "picture")
    //img.frame = CGRect(x: 0, y: 0, width: 70, height: 70)
    //img.layer.cornerRadius = img.frame.width / 2
    img.clipsToBounds = true
    
    let imageClicked = UITapGestureRecognizer(target: self, action: #selector(hasTappedImage))
    img.addGestureRecognizer(imageClicked)
    
    pickerOne.delegate = self
    //pickerTwo.delegate = self
    pickerOne.dataSource = self
    //pickerTwo.dataSource = dataSource
    
    
  }
  
  func addNewDevice(){
    
    //, "Image" : img.image as AnyObject
    uploadImage()
    print("Saving")
    
  }
}
extension newDeviceViewController : UIPickerViewDelegate, UIPickerViewDataSource {
  func numberOfComponents(in pickerView: UIPickerView) -> Int {
    return 1
  }
  
  func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
    return dataSource.count
  }
  func pickerView(_ pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
    return dataSource[row]
  }
  func pickerView(_ pickerView: UIPickerView, didSelectRow row: Int, inComponent component: Int) {
    rowSelected = dataSource[row]
  }
  @objc func hasTappedImage () {
    print("TAPPED")
    let refreshAlert = UIAlertController(title: "Add Image", message: "", preferredStyle: UIAlertController.Style.alert)
    
    refreshAlert.addAction(UIAlertAction(title: "Take Photo", style: .default, handler: { (action: UIAlertAction!) in
      print("Camera")
      let image = UIImagePickerController()
      image.delegate = self
      image.sourceType = UIImagePickerController.SourceType.camera
      image.allowsEditing = false
      self.present(image, animated: true){}
    }))
    
    refreshAlert.addAction(UIAlertAction(title: "Upload Image", style: .default, handler: { (action: UIAlertAction!) in
      print("Uploading")
      
      let image = UIImagePickerController()
      image.delegate = self
      image.sourceType = UIImagePickerController.SourceType.photoLibrary
      image.allowsEditing = false
      
      self.present(image, animated: true){}
      
    }))
    self.present(refreshAlert, animated: true, completion: nil)
    
    print("UNTAPPED")
    
  }
  func imagePickerController(_ picker: UIImagePickerController, didFinishPickingMediaWithInfo info: [UIImagePickerController.InfoKey : Any]) {
    if let image = info[UIImagePickerController.InfoKey.originalImage] as? UIImage {
      let imageData = image.jpeg(.lowest)
      img.image = UIImage(data:imageData!,scale:0.5)!.resizeWithPercent(percentage: 0.2)
    }
    self.dismiss(animated: true, completion: nil)
    
  }
  func uploadImage(){
    let imageName = UUID().uuidString
    let storageRef = Storage.storage().reference().child("device_images").child("\(imageName).png")
    print("Has been called")
    if let uploadData = self.img.image?.pngData() {
      print("2")
      
      storageRef.putData(uploadData, metadata: nil, completion: { (_, error) in
        
        if error != nil {
          print("error")
          
        }else {
          print("3")
        
        storageRef.downloadURL(completion: { (url, err) in
          if let err = err {
            print(err)
            return
          }
          print("3")
          guard let url = url else { return }
          let location = self.textFieldTwo.text
          let name = self.textFieldOne.text
          let device : [String : AnyObject] = ["Name" : name as AnyObject, "Location" : location as AnyObject, "Motion" : self.rowSelected as AnyObject, "bleId" : self.bleId as AnyObject, "profileImageUrl": url.absoluteString as AnyObject]
          print("4")
          let myDataBase = Database.database().reference()
          myDataBase.child("Devices").childByAutoId().setValue(device)
        })
        }
      })
    }
  }
}



