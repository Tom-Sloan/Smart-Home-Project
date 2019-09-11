//
//  StepCell.swift
//  tableView
//
//  Created by Thomas Sloan on 2019-07-06.
//  Copyright © 2019 Thomas Sloan. All rights reserved.

import UIKit

class StepCell: UITableViewCell {
  var shapeLayer = CAShapeLayer()
  var trackLayer = CAShapeLayer()
  var pulsatingLayer: CAShapeLayer!
  
  @IBOutlet var myView: UIView!
  @IBOutlet var horizontalStack: UIStackView!
  @IBOutlet var vStackOne: UIStackView!
  @IBOutlet var vStackTwo: UIStackView!
  @IBOutlet var vStackThree: UIStackView!
  
  @IBOutlet var titleViewOne: UIView!
  @IBOutlet var titleViewTwo: UIView!
  @IBOutlet var titleViewThree: UIView!
  
  @IBOutlet var titleLabelOne: UILabel!
  @IBOutlet var titleLabelTwo: UILabel!
  @IBOutlet var titleLabelThree: UILabel!
  
  @IBOutlet var contentViewOne: UIView!
  @IBOutlet var contentViewTwo: UIView!
  @IBOutlet var contentViewThree: UIView!
  
  @IBOutlet var contentLabelOne: UILabel!
  @IBOutlet var contentLabelTwo: UILabel!
  @IBOutlet var contentLabelThree: UILabel!
  var numberOfSteps = 10
  
  //  let heartBeat: UILabel = {
  //    let label = UILabel()
  //    label.text = "80"
  //    label.textAlignment = .center
  //    label.font = UIFont.boldSystemFont(ofSize: 17)
  //    label.textColor = .black
  //    return label
  //  }()
  //  let steps: UILabel = {
  //    let label = UILabel()
  //    label.text = "10,000"
  //    label.textAlignment = .center
  //    label.font = UIFont.boldSystemFont(ofSize: 17)
  //    label.textColor = .black
  //    return label
  //  }()
  //  func setUpInfo(device : Test){
  //    heartBeat.text = "89"
  //  }
  
  
  override func awakeFromNib() {
    
    super.awakeFromNib()
    
    
  }
  func removeBackGroundColors(){
    contentViewOne.backgroundColor = .clear
    contentViewTwo.backgroundColor = .clear
    contentViewThree.backgroundColor = .clear
    titleViewOne.backgroundColor = .clear
    titleViewTwo.backgroundColor = .clear
    titleViewThree.backgroundColor = .clear
  }
  
  
  @objc func sayHello(){
    animatePulsatingLayer()
    print("PULSE")
  }
  
  private func animatePulsatingLayer() {
    //    //Need to get correct keypath, i think this is it though
    let animation = CABasicAnimation(keyPath: "transform.scale")
    
    animation.toValue = 1.2
    animation.duration = 60/80-(60/80)/2
    animation.timingFunction = CAMediaTimingFunction(name: CAMediaTimingFunctionName.easeOut)
    animation.autoreverses = true
    
    pulsatingLayer.add(animation, forKey: "pulsing")
  }
  var once = true
  override func layoutSubviews() {
    super.layoutSubviews()
    contentLabelOne.text = String(numberOfSteps)
    if once {
      setupCircleLayers()
      once = false
      shapeLayer.strokeEnd = CGFloat(numberOfSteps) / CGFloat(10000)
    }
  }
  func setupCircleLayers() {
    
    trackLayer = createCircleShapeLayer(strokeColor: .lightGray, fillColor: .white, uiview: contentViewOne)
    trackLayer.position = CGPoint(x:contentViewOne.frame.width/2, y:contentViewOne.frame.height/2.0 - 10)
    contentViewOne.layer.addSublayer(trackLayer)
    
    shapeLayer = createCircleShapeLayer(strokeColor: .pjRed ?? .blue, fillColor: .clear, uiview: contentViewOne)
    
    shapeLayer.strokeEnd = 0
    shapeLayer.position = CGPoint(x:contentViewOne.frame.width/2 , y:contentViewOne.frame.height/2.0 - 10 )
    contentViewOne.layer.addSublayer(shapeLayer)
    
    contentViewOne.addGestureRecognizer(UITapGestureRecognizer(target: self, action: #selector(handleTap)))
    
    pulsatingLayer = createCircleShapeLayer(strokeColor: .pjRed ?? .blue, fillColor: .clear, uiview: contentViewTwo)
    pulsatingLayer.position = CGPoint(x:contentViewTwo.frame.width/2 + 2, y:contentViewTwo.frame.height/2.0 - 10)
    contentViewTwo.layer.addSublayer(pulsatingLayer)
    //contentViewTwo.layer.addSublayer(createCircleShapeLayer(strokeColor: .white, fillColor: .white, uiview: contentViewTwo))
    
    
    animatePulsatingLayer()
    Timer.scheduledTimer(timeInterval: 60/80, target: self, selector: #selector(sayHello), userInfo: nil, repeats: true)
    removeBackGroundColors()
  }
  private func createCircleShapeLayer(strokeColor: UIColor, fillColor: UIColor, uiview : UIView) -> CAShapeLayer {
    let layer = CAShapeLayer()
    
    //NOTE: to change start location, circle goes clockwise not counter therfore (0, -1) is pi/2 etc
    let circularPath = UIBezierPath(arcCenter: .zero, radius: 40, startAngle: 3*CGFloat.pi/2, endAngle: 7 * CGFloat.pi/2, clockwise: true)
    layer.path = circularPath.cgPath
    layer.strokeColor = strokeColor.cgColor
    layer.lineWidth = 20
    layer.fillColor = fillColor.cgColor
    layer.lineCap = CAShapeLayerLineCap.round
    
    return layer
  }
  
  @objc private func handleTap() {
    //    print("Attempting to animate stroke")
    //    let basicAnimation = CABasicAnimation(keyPath: "strokeEnd")
    //    basicAnimation.toValue = 1
    //    basicAnimation.duration = 2
    //    basicAnimation.fillMode = CAMediaTimingFillMode.forwards
    //    basicAnimation.isRemovedOnCompletion = false
    //    shapeLayer.add(basicAnimation, forKey: "urSoBasic")
    
  }
  
  override func setSelected(_ selected: Bool, animated: Bool) {
    super.setSelected(selected, animated: animated)
    
    // Configure the view for the selected state
  }
  
}











