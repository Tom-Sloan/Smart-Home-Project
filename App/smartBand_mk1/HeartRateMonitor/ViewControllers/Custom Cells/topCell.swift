
import UIKit

class topCell: UITableViewCell {

  
    @IBOutlet var view: UIView!
  var shapeLayer = CAShapeLayer()
    override func awakeFromNib() {
        super.awakeFromNib()
      setupCircleLayers()
      
      view.addGestureRecognizer(UITapGestureRecognizer(target: self, action: #selector(handleTap)))
      
    }
  
  private func setupCircleLayers() {
    
    let trackLayer = createCircleShapeLayer(strokeColor: .lightGray, fillColor: .white)
    view.layer.addSublayer(trackLayer)
    shapeLayer = createCircleShapeLayer(strokeColor: UIColor(hex: "#f23557F8") ?? .blue, fillColor: .clear)
    shapeLayer.transform = CATransform3DMakeRotation(-CGFloat.pi / 2, 0, 0, 1)
    shapeLayer.strokeEnd = 0
    view.layer.addSublayer(shapeLayer)
  }
  private func createCircleShapeLayer(strokeColor: UIColor, fillColor: UIColor) -> CAShapeLayer {
    let layer = CAShapeLayer()
    let circularPath = UIBezierPath(arcCenter: .zero, radius: 40, startAngle: 0, endAngle: 2 * CGFloat.pi, clockwise: true)
    layer.path = circularPath.cgPath
    layer.strokeColor = strokeColor.cgColor
    layer.lineWidth = 20
    layer.fillColor = fillColor.cgColor
    layer.lineCap = CAShapeLayerLineCap.round
    //layer.position = view.center
    return layer
  }
  
  @objc private func handleTap() {
    print("Attempting to animate stroke")
    
    let basicAnimation = CABasicAnimation(keyPath: "strokeEnd")
    
    basicAnimation.toValue = 1
    
    basicAnimation.duration = 2
    
    basicAnimation.fillMode = CAMediaTimingFillMode.forwards
    basicAnimation.isRemovedOnCompletion = false
    
    shapeLayer.add(basicAnimation, forKey: "urSoBasic")
    
  }



    override func setSelected(_ selected: Bool, animated: Bool) {
        super.setSelected(selected, animated: animated)

        // Configure the view for the selected state
    }
    
}
