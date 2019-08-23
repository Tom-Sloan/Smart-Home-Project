//
//  TableViewCell.swift
//  tableView
//
//  Created by Thomas Sloan on 2019-07-05.
//  Copyright © 2019 Thomas Sloan. All rights reserved.
//

import UIKit

protocol btnEvent {
    func didTapRight(boo : Bool)
    func didTapLeft(boo : Bool)
}


class TableViewCell: UITableViewCell {
    
    
    @IBOutlet var titles: UIView!
    @IBOutlet var information: UIView!
    
    
    @IBOutlet var secondContentView: UIView!
    @IBOutlet var infoView: UIView!
    @IBOutlet var titleView: UIView!
    @IBOutlet var secondTitleView: UIView!
    @IBOutlet var secondLabel: UILabel!
    @IBOutlet var myView: UIView!
    @IBOutlet var label: UILabel!
    
    var delegate: btnEvent?
    var pulsatingLayer: CAShapeLayer!
    
    let heartBeat: UILabel = {
        let label = UILabel()
        label.text = "80"
        label.textAlignment = .center
        label.font = UIFont.boldSystemFont(ofSize: 25)
        label.textColor = .white
        return label
    }()
    
    override func awakeFromNib() {
        super.awakeFromNib()
        // Initialization code
        setUpViews()
        
        pulsatingLayer = createCircleShapeLayer(strokeColor: .clear, fillColor: UIColor(hex: "#f23557ff") ?? .blue)
        infoView.layer.addSublayer(pulsatingLayer)
        let trackLayer = createCircleShapeLayer(strokeColor:  UIColor(hex: "#3b4a6bFF") ?? .blue, fillColor:  UIColor(hex: "#3b4a6bFF") ?? .blue)
        infoView.layer.addSublayer(trackLayer)
        setupPercentageLabel()
        animatePulsatingLayer()
        
        
        //to stop gameTimer?.invalidate()
        Timer.scheduledTimer(timeInterval: 60/80, target: self, selector: #selector(sayHello), userInfo: nil, repeats: true)
        
    }
    private func setUpViews(){
        setUpOuter()
        setClear()
        //addConst()
        addBorders()
        addGestures()
        
        label.textAlignment = .left
        secondLabel.textAlignment = .left
        label.textColor = .white
        secondLabel.textColor = .white
        
    }
    @objc func sayHello(){
        animatePulsatingLayer()
        print("PULSE")
    }
    private func setupPercentageLabel() {
        infoView.addSubview(heartBeat)
        heartBeat.frame = CGRect(x: 0, y: 0, width: 100, height: 100)
        heartBeat.center = infoView.center
    }
    private func animatePulsatingLayer() {
        let animation = CABasicAnimation(keyPath: "transform.scale")
        
        animation.toValue = 1.5
        animation.duration = 60/80-(60/80)/2
        animation.timingFunction = CAMediaTimingFunction(name: CAMediaTimingFunctionName.easeOut)
        animation.autoreverses = true
        //animation.repeatCount = Float.infinity
        
        pulsatingLayer.add(animation, forKey: "pulsing")
    }

    override func setSelected(_ selected: Bool, animated: Bool) {
        super.setSelected(selected, animated: animated)
    }
    
    @objc func hasTappedLeft(){
        delegate?.didTapLeft(boo: true)
    }
    @objc func hasTappedRight(){
        delegate?.didTapRight(boo: true)
    }
    fileprivate func setUpOuter() {
        // The subview inside the collection view cell
        myView.layer.backgroundColor = UIColor(hex: "#3b4a6bFF")?.cgColor
        myView.layer.cornerRadius = 20.0
        myView.layer.shadowColor = UIColor.gray.cgColor
        myView.layer.shadowOffset = CGSize(width: 0.0, height: 0.0)
        myView.layer.shadowRadius = 5.0
        myView.layer.shadowOpacity = 0.7
    
    }
    
    fileprivate func setClear() {
        titles.backgroundColor = UIColor.clear
        information.backgroundColor = UIColor.clear
        
    }
    
    fileprivate func addGestures() {
        let tapGestureLeft = UITapGestureRecognizer(target: self, action: #selector(hasTappedLeft))
        infoView.addGestureRecognizer(tapGestureLeft)
        let tapGestureLeftTop = UITapGestureRecognizer(target: self, action: #selector(hasTappedLeft))
        titleView.addGestureRecognizer(tapGestureLeftTop)
        
        let tapGestureRight = UITapGestureRecognizer(target: self, action: #selector(hasTappedRight))
        secondContentView.addGestureRecognizer(tapGestureRight)
        
        let tapGestureRightTop = UITapGestureRecognizer(target: self, action: #selector(hasTappedRight))
        secondTitleView.addGestureRecognizer(tapGestureRightTop)
    }
    

    private func addConst(){
        self.infoView.frame.size.width = self.information.frame.size.width/2
        self.titleView.frame.size.width = self.titles.frame.size.width/2
        self.secondContentView.frame.size.width = self.information.frame.size.width/2
        self.secondTitleView.frame.size.width = self.titles.frame.size.width/2
        
        print("Titles:  \(self.titles.frame.size.width/2) secondT \(self.secondTitleView.frame.size.width)")
        
        
    }
    private func addBorders(){
        let width = CGFloat(1)
        secondTitleView.addLeftBorder(color: UIColor.lightGray, borderWidth: width)
        secondContentView.addLeftBorder(color: UIColor.lightGray, borderWidth: width)
        infoView.addRightBorder(color: UIColor.lightGray, borderWidth: width)
        titleView.addRightBorder(color: UIColor.lightGray, borderWidth: width)
    }
//    private func setupNotificationObservers() {
//        NotificationCenter.default.addObserver(self, selector: #selector(handleEnterForeground), name: .UIApplication.willEnterForegroundNotification, object: nil)
//    }

    @objc private func handleEnterForeground() {
        animatePulsatingLayer()
    }
    private func createCircleShapeLayer(strokeColor: UIColor, fillColor: UIColor) -> CAShapeLayer {
        let layer = CAShapeLayer()
        let circularPath = UIBezierPath(arcCenter: .zero, radius: 50, startAngle: 0, endAngle: 2 * CGFloat.pi, clockwise: true)
        layer.path = circularPath.cgPath
        layer.strokeColor = strokeColor.cgColor
        layer.lineWidth = 20
        layer.fillColor = fillColor.cgColor
        layer.lineCap = CAShapeLayerLineCap.round
        layer.position = infoView.center
        return layer
    }
    
}

//Dont forget alpha
extension UIColor {
    public convenience init?(hex: String) {
        let r, g, b, a: CGFloat
        
        if hex.hasPrefix("#") {
            let start = hex.index(hex.startIndex, offsetBy: 1)
            let hexColor = String(hex[start...])
            
            if hexColor.count == 8 {
                let scanner = Scanner(string: hexColor)
                var hexNumber: UInt64 = 0
                
                if scanner.scanHexInt64(&hexNumber) {
                    r = CGFloat((hexNumber & 0xff000000) >> 24) / 255
                    g = CGFloat((hexNumber & 0x00ff0000) >> 16) / 255
                    b = CGFloat((hexNumber & 0x0000ff00) >> 8) / 255
                    a = CGFloat(hexNumber & 0x000000ff) / 255
                    
                    self.init(red: r, green: g, blue: b, alpha: a)
                    return
                }
            }
        }
        
        return nil
    }
    
    
}
extension UIView{
    func addTopBorder(color: UIColor?, borderWidth: CGFloat) {
        let border = UIView()
        border.backgroundColor = color
        border.autoresizingMask = [.flexibleWidth, .flexibleBottomMargin]
        border.frame = CGRect(x: 0, y: 0, width: frame.size.width, height: borderWidth)
        addSubview(border)
    }
    
    func addBottomBorder(color: UIColor?, borderWidth: CGFloat) {
        let border = UIView()
        border.backgroundColor = color
        border.autoresizingMask = [.flexibleWidth, .flexibleTopMargin]
        border.frame = CGRect(x: 0, y: frame.size.height - borderWidth, width: frame.size.width, height: borderWidth)
        addSubview(border)
    }
    
    func addLeftBorder(color: UIColor?, borderWidth: CGFloat) {
        let border = UIView()
        border.backgroundColor = color
        border.frame = CGRect(x: 0, y: 0, width: borderWidth, height: frame.size.height)
        border.autoresizingMask = [.flexibleHeight, .flexibleRightMargin]
        addSubview(border)
    }
    
    func addRightBorder(color: UIColor?, borderWidth: CGFloat) {
        let border = UIView()
        border.backgroundColor = color
        border.autoresizingMask = [.flexibleHeight, .flexibleLeftMargin]
        border.frame = CGRect(x: frame.size.width - borderWidth, y: 0, width: borderWidth, height: frame.size.height)
        addSubview(border)
    }
}
