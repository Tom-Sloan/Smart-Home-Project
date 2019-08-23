
import UIKit

protocol buttonDelegate {
  func didTapButton(bool: Bool)
}

class CustomCell: UITableViewCell {
  
    @IBOutlet var videoImageView: UIImageView!
    var videoItem: Video!
    var delegate: buttonDelegate?
    
    func setVideo(video: Video) {
        videoItem = video
        videoImageView.image = video.image
    }
    
    @IBAction func button(_ sender: YTRoundedButton) {
        print("Button tapped")
        delegate?.didTapButton(bool: true)
    }
  
}
/*
 var message : String?
 var mainImage : UIImage?
 var delegate : buttonDelegate?
 
 
 
 
 var messageView : UITextView = {
 var textView = UITextView()
 textView.isScrollEnabled = false
 textView.translatesAutoresizingMaskIntoConstraints = false
 return textView
 }()
 var mainImageView : UIImageView = {
 var imageView = UIImageView()
 imageView.translatesAutoresizingMaskIntoConstraints = false
 return imageView
 }()
 
 override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
 super.init(style: style, reuseIdentifier: reuseIdentifier)
 
 
 self.addSubview(mainImageView)
 self.addSubview(messageView)
 
 mainImageView.leftAnchor.constraint(equalTo: self.leftAnchor).isActive = true
 mainImageView.topAnchor.constraint(equalTo: self.topAnchor).isActive = true
 mainImageView.bottomAnchor.constraint(equalTo: self.bottomAnchor).isActive = true
 mainImageView.widthAnchor.constraint(equalToConstant: 100).isActive = true
 mainImageView.heightAnchor.constraint(equalToConstant: 100).isActive = true
 
 messageView.leftAnchor.constraint(equalTo: self.mainImageView.rightAnchor).isActive = true
 messageView.topAnchor.constraint(equalTo: self.topAnchor).isActive = true
 messageView.bottomAnchor.constraint(equalTo: self.bottomAnchor).isActive = true
 messageView.rightAnchor.constraint(equalTo: self.rightAnchor).isActive = true
 
 }
 
 override func layoutSubviews() {
 super.layoutSubviews()
 if let message = message {
 messageView.text = message
 
 }
 if let image = mainImage {
 mainImageView.image = image
 }
 }
 
 required init?(coder aDecoder: NSCoder) {
 fatalError("init(coder:) has not been implemented")
 }
 
 @objc func buttonAction(sender: UIButton!) {
 print("Button tapped")
 delegate?.didTapButton(bool: true)
 }
 */
