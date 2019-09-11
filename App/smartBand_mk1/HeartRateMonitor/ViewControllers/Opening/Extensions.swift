//
//  Extensions.swift
//  gameofchats
//
//  Created by Brian Voong on 7/5/16.
//  Copyright © 2016 letsbuildthatapp. All rights reserved.
//

import UIKit

let imageCache = NSCache<NSString, AnyObject>()

extension UIImageView {
  
  func loadImageUsingCacheWithUrlString(_ urlString: String) {
    
    self.image = nil
    
    //check cache for image first
    if let cachedImage = imageCache.object(forKey: urlString as NSString) as? UIImage {
      self.image = cachedImage
      return
    }
    
    //otherwise fire off a new download
    let url = URL(string: urlString)
    print("URLstring: \(urlString)")
    print("URL: \(url)")

    URLSession.shared.dataTask(with: url!, completionHandler: { (data, response, error) in
      
      //download hit an error so lets return out
      if let error = error {
        print(error)
        return
      }
      
      DispatchQueue.main.async(execute: {
        
        if let downloadedImage = UIImage(data: data!) {
          imageCache.setObject(downloadedImage, forKey: urlString as NSString)
          
          self.image = downloadedImage
        }
      })
      
    }).resume()
  }
  
}
extension UIImage {
  enum JPEGQuality: CGFloat {
    case lowest  = 0
    case low     = 0.25
    case medium  = 0.5
    case high    = 0.75
    case highest = 1
  }
  
  /// Returns the data for the specified image in JPEG format.
  /// If the image object’s underlying image data has been purged, calling this function forces that data to be reloaded into memory.
  /// - returns: A data object containing the JPEG data, or nil if there was a problem generating the data. This function may return nil if the image has no data or if the underlying CGImageRef contains data in an unsupported bitmap format.
  func jpeg(_ jpegQuality: JPEGQuality) -> Data? {
    return jpegData(compressionQuality: jpegQuality.rawValue)
  }
  
  func resizeWithPercent(percentage: CGFloat) -> UIImage? {
    let imageView = UIImageView(frame: CGRect(origin: .zero, size: CGSize(width: size.width * percentage, height: size.height * percentage)))
    imageView.contentMode = .scaleAspectFit
    imageView.image = self
    UIGraphicsBeginImageContextWithOptions(imageView.bounds.size, false, scale)
    guard let context = UIGraphicsGetCurrentContext() else { return nil }
    imageView.layer.render(in: context)
    guard let result = UIGraphicsGetImageFromCurrentImageContext() else { return nil }
    UIGraphicsEndImageContext()
    return result
  }
  func resizeWithWidth(width: CGFloat) -> UIImage? {
    let imageView = UIImageView(frame: CGRect(origin: .zero, size: CGSize(width: width, height: CGFloat(ceil(width/size.width * size.height)))))
    imageView.contentMode = .scaleAspectFit
    imageView.image = self
    UIGraphicsBeginImageContextWithOptions(imageView.bounds.size, false, scale)
    guard let context = UIGraphicsGetCurrentContext() else { return nil }
    imageView.layer.render(in: context)
    guard let result = UIGraphicsGetImageFromCurrentImageContext() else { return nil }
    UIGraphicsEndImageContext()
    return result
  }
}
class CheckButton : UIButton {
  func check(_ yn:Bool) {
    self.setTitle(yn ? "✔" : "", for: .normal)
  }
  override init(frame:CGRect) {
    super.init(frame:frame)
    setUpButton()
  }
  required init?(coder aDecoder: NSCoder) {
    super.init(coder: aDecoder)
    setUpButton()
  }
  private func setUpButton(){
  
    self.layer.borderWidth = 2
    self.layer.cornerRadius = 10
    self.titleLabel?.font = UIFont(name:"Georgia", size:10)
    self.setTitleColor(.black, for: .normal)
    self.check(false)
  }
}

//Dont forget alpha
extension UIColor {
  static let bgColor = UIColor(hex: "#dadada1f")
  static let pjRed = UIColor(hex: "#f23557ff")
  static let pjYellow = UIColor(hex: "#f0d43aff")
  static let pjLightBlue = UIColor(hex: "#22b2daff")
  static let pjDarkBlue = UIColor(hex: "#3b4a6bff")
  
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
extension UIView {
  
  func roundCorners(_ corners: UIRectCorner, radius: CGFloat) {
    let path = UIBezierPath(roundedRect: self.bounds, byRoundingCorners: corners, cornerRadii: CGSize(width: radius, height: radius))
    let mask = CAShapeLayer()
    mask.path = path.cgPath
    self.layer.mask = mask
  }
  
}
