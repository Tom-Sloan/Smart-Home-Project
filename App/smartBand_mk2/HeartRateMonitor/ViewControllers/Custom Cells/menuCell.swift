

import UIKit

protocol segueFunc {
  func handleSegue(location: String)
}

class menuCell: UITableViewCell {

    
    @IBOutlet var iconView: UIImageView!
    @IBOutlet var titleLabel: UILabel!
    @IBOutlet var arrowLabel: UILabel!
    @IBOutlet var view: UIView!
  
  var delagte : segueFunc?
    override func awakeFromNib() {
        super.awakeFromNib()
      print("DONE")
        // Initialization code
      
      let tapGesture = UITapGestureRecognizer(target: self, action: #selector(handleTap))
      view.addGestureRecognizer(tapGesture)
      print("OK")
    }
  
    @objc func handleTap(){
      print("OK")
      delagte?.handleSegue(location: titleLabel.text ?? "About")
      
    }

    override func setSelected(_ selected: Bool, animated: Bool) {
        super.setSelected(selected, animated: animated)

        // Configure the view for the selected state
    }
  override func gestureRecognizer(_ gestureRecognizer: UIGestureRecognizer, shouldReceive touch: UITouch) -> Bool {
    return touch.view == gestureRecognizer.view
  }
}
