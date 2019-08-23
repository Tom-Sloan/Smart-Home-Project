//
//  TableViewCell.swift
//  tableView
//
//  Created by Thomas Sloan on 2019-07-05.
//  Copyright © 2019 Thomas Sloan. All rights reserved.
//
// weightlighting icon: https://www.flaticon.com/authors/elias-bikbulatov
// Neditation Icon https://www.flaticon.com/authors/ddara" title="dDara"
//Running "https://www.flaticon.com/authors/freepik" title="Freepik"


import UIKit

protocol btnEvent {
  func didTapRight(boo : Bool)
  func didTapLeft(boo : Bool)
}


class TableViewCell: UITableViewCell {
  
  @IBOutlet var myView: UIView!
  @IBOutlet var titles: UIView!
  @IBOutlet var information: UIView!
  
  @IBOutlet var secondContentView: UIView!
  @IBOutlet var infoView: UIView!
  @IBOutlet var thirdContentView: UIView!
  
  
  @IBOutlet var title: UILabel!
  @IBOutlet var firstImage: UIImageView!
  @IBOutlet var secondImage: UIImageView!
  @IBOutlet var thirdImage: UIImageView!
  
  @IBOutlet var firstLabel: UILabel!
  @IBOutlet var secondLabel: UILabel!
  @IBOutlet var thirdLabel: UILabel!
  
  
  var delegate: btnEvent?
  //  var pulsatingLayer: CAShapeLayer!
  
  let heartBeat: UILabel = {
    let label = UILabel()
    label.text = "80"
    label.textAlignment = .center
    label.font = UIFont.boldSystemFont(ofSize: 25)
    label.textColor = .black
    return label
  }()
  
  override func awakeFromNib() {
    super.awakeFromNib()
    // Initialization code
    setUpViews()
    
    
    //    pulsatingLayer = createCircleShapeLayer(strokeColor: .clear, fillColor: UIColor(hex: "#f23557ff") ?? .blue)
    //    infoView.layer.addSublayer(pulsatingLayer)
    //    let trackLayer = createCircleShapeLayer(strokeColor:  UIColor(hex: "#3b4a6bFF") ?? .blue, fillColor:  UIColor(hex: "#3b4a6bFF") ?? .blue)
    //    infoView.layer.addSublayer(trackLayer)
    //    setupPercentageLabel()
    //    animatePulsatingLayer()
    
    
    //to stop gameTimer?.invalidate()
    //Timer.scheduledTimer(timeInterval: 60/80, target: self, selector: #selector(sayHello), userInfo: nil, repeats: true)
    
  }

//  override func layoutSubviews() {
//    super.layoutSubviews()
//
//    infoView.roundCorners([.topLeft, .bottomLeft], radius: 10)
//  }
  private func setUpViews(){
    setUpOuter()
    setClear()
    //addConst()
    //addBorders()
    addGestures()
    
    title.textAlignment = .left
    
    title.textColor = .black
    
    
  }
  //  @objc func sayHello(){
  //    animatePulsatingLayer()
  //    print("PULSE")
  //  }
  //  private func setupPercentageLabel() {
  //    infoView.addSubview(heartBeat)
  //    heartBeat.frame = CGRect(x: 0, y: 0, width: 100, height: 100)
  //    heartBeat.center = infoView.center
  //  }
  //  private func animatePulsatingLayer() {
  //    let animation = CABasicAnimation(keyPath: "transform.scale")
  //
  //    animation.toValue = 1.5
  //    animation.duration = 60/80-(60/80)/2
  //    animation.timingFunction = CAMediaTimingFunction(name: CAMediaTimingFunctionName.easeOut)
  //    animation.autoreverses = true
  //    //animation.repeatCount = Float.infinity
  //
  //    pulsatingLayer.add(animation, forKey: "pulsing")
  //  }
  //
  //  override func setSelected(_ selected: Bool, animated: Bool) {
  //    super.setSelected(selected, animated: animated)
  //  }
  
  @objc func hasTappedLeft(){
    delegate?.didTapLeft(boo: true)
  }
  @objc func hasTappedRight(){
    delegate?.didTapRight(boo: true)
  }
  fileprivate func setUpOuter() {
    // The subview inside the collection view cell
    myView.layer.backgroundColor = UIColor.white.cgColor
    myView.layer.cornerRadius = 20.0
    myView.layer.shadowColor = UIColor.gray.cgColor
    myView.layer.shadowOffset = CGSize(width: 0.0, height: 0.0)
    myView.layer.shadowRadius = 5.0
    myView.layer.shadowOpacity = 0.7
    infoView.clipsToBounds = true
    infoView.layer.cornerRadius = 20
    infoView.layer.maskedCorners = [.layerMinXMaxYCorner]
    thirdContentView.clipsToBounds = true
    thirdContentView.layer.cornerRadius = 20
    thirdContentView.layer.maskedCorners = [.layerMaxXMaxYCorner]
    //addBorders()
    
  }
  
  fileprivate func setClear() {
    titles.backgroundColor = UIColor.clear
    information.backgroundColor = UIColor.clear
    
  }
  
  fileprivate func addGestures() {
    let tapGestureLeft = UITapGestureRecognizer(target: self, action: #selector(hasTappedLeft))
    infoView.addGestureRecognizer(tapGestureLeft)
    
    let tapGestureRight = UITapGestureRecognizer(target: self, action: #selector(hasTappedRight))
    secondContentView.addGestureRecognizer(tapGestureRight)
    
    
  }
  
  
  private func addConst(){
    self.infoView.frame.size.width = self.information.frame.size.width/2
    
    self.secondContentView.frame.size.width = self.information.frame.size.width/2
    
    
    print("Titles:  \(self.titles.frame.size.width/2) :")
    
    
  }
  private func addBorders(){
    let width = CGFloat(2)
    titles.addBottomBorder(color: UIColor.black, borderWidth: width)
    
  }
  
  //  @objc private func handleEnterForeground() {
  //    animatePulsatingLayer()
  //  }
  //  private func createCircleShapeLayer(strokeColor: UIColor, fillColor: UIColor) -> CAShapeLayer {
  //    let layer = CAShapeLayer()
  //    let circularPath = UIBezierPath(arcCenter: .zero, radius: 50, startAngle: 0, endAngle: 2 * CGFloat.pi, clockwise: true)
  //    layer.path = circularPath.cgPath
  //    layer.strokeColor = strokeColor.cgColor
  //    layer.lineWidth = 20
  //    layer.fillColor = fillColor.cgColor
  //    layer.lineCap = CAShapeLayerLineCap.round
  //    layer.position = infoView.center
  //    return layer
  //  }
  
}


