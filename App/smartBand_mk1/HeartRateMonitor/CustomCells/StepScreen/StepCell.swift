//
//  StepCell.swift
//  tableView
//
//  Created by Thomas Sloan on 2019-07-06.
//  Copyright © 2019 Thomas Sloan. All rights reserved.
//

import UIKit

class StepCell: UITableViewCell {
    var shapeLayer = CAShapeLayer()
 
    @IBOutlet var myView: UIView!
    @IBOutlet var view: UIView!
    @IBOutlet var secondView: UIView!
    @IBOutlet var thirdView: UIView!
    
    @IBOutlet var outerViewOne: UIView!
    @IBOutlet var outerViewTwo: UIView!
    @IBOutlet var outerViewThree: UIView!
    
    @IBOutlet var stackOne: UIStackView!
    @IBOutlet var stackTwo: UIStackView!
    @IBOutlet var stackThree: UIStackView!
    
    @IBOutlet var titleOne: UILabel!
    @IBOutlet var titleTwo: UILabel!
    @IBOutlet var titleThree: UILabel!
    
    override func awakeFromNib() {
        
        super.awakeFromNib()
        //setupNotificationObservers()
        
        view.backgroundColor = UIColor.white
        
        setupCircleLayers()
        
        view.addGestureRecognizer(UITapGestureRecognizer(target: self, action: #selector(handleTap)))
        
       // Set up outer View
        outerView()

    
    }
//    private func setupNotificationObservers() {
//        NotificationCenter.default.addObserver(self, selector: #selector(handleEnterForeground), name: .UIApplication.willEnterForegroundNotification, object: nil)
//    }
//
//    @objc private func handleEnterForeground() {
//
//    }
    
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
        layer.position = view.center
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
    func outerView(){
        
        outerViewOne.layer.cornerRadius = 20.0
        outerViewTwo.layer.cornerRadius = 20.0
        outerViewThree.layer.cornerRadius = 20.0
        
        
        
        myView.layer.cornerRadius = 20.0
        myView.layer.shadowColor = UIColor.gray.cgColor
        myView.layer.shadowOffset = CGSize(width: 0.0, height: 0.0)
        myView.layer.shadowRadius = 5.0
        myView.layer.shadowOpacity = 0.7
    }
    
    
}




