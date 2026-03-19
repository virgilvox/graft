// Type stubs for optional peer dependencies
declare module 'serialport' {
  export class SerialPort {
    constructor(options: any)
    on(event: string, handler: (...args: any[]) => void): void
    write(data: any, callback?: (err?: Error) => void): void
    close(callback?: (err?: Error) => void): void
    isOpen: boolean
  }
}

declare module 'mqtt' {
  export function connect(url: string, options?: any): any
  export function connectAsync(url: string, options?: any): Promise<any>
}
